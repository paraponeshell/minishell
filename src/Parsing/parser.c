/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:20:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 01:55:09 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		printf("Split[%d]: %s\n", i, split[i]);
		i++;
	}
}

int	is_op_correct(char *str)
{
	int	i;
	int	d_quotes;
	int	s_quotes;
	int	op;

	i = 0;
	d_quotes = 0;
	s_quotes = 0;
	if (str == NULL)
		return (1);
	while (str[i] != '\0')
	{
		if (str[i] == '"')
			d_quotes = !d_quotes;
		else if (str[i] == '\'')
			s_quotes = !s_quotes;
		if (d_quotes == 0 && s_quotes == 0)
		{
			op = 0;
			while (str [i] != '\0' && (str[i] == '|' || str[i] == '<' || str[i] == '>' || str[i] == '&'))
			{
				op++;
				i++;
			}
			if (op > 2)
				return (1);
		}
		i++;
		if (i >= ft_strlen(str))
			return (0);
	}
	return (0);
}
void	parser(char *str, t_mini *mini)
{
	char		**splitted;
	int			*operator;
	int			exit_status;

	if (is_op_correct(str) == 1)
	{
		printf("Error: too many operators\n");
		return ;
	}
	splitted = first_split(str);
	if (splitted == NULL)
		return ;
	operator = get_operators(str);
	if (putlist(mini, splitted, operator) == 1)
	{
		//print_split(splitted);
		free_split(splitted);
		free(operator);
		printf("Error: ambiguous redirectiond\n");
		return ;
	}
	free_split(splitted);
	free(operator);
	add_red_to_env(&mini->redirection, &mini->env);
	add_cmd_to_env(&mini->commands, &mini->env);
	//printf("Before createpipes:\n");
	//print_commands(mini->commands);
	//if (valid_line(mini->commands, mini->redirection) == 0)
		createpipes(mini->commands, mini->redirection, mini->env);
	block_signal(SIGQUIT);
	while (wait(&exit_status) > 0)
		add_exit_status(exit_status, &mini->env);
	//printf("After parser:\n");
	//print_commands(mini->commands);
	free_cmd_red(&mini->commands, &mini->redirection);
}

int	putlist(t_mini	*mini, char **split, int *op)
{
	char		*buffer;
	t_commands	**cmds;
	t_commands	*current;
	int			i;

	cmds = &mini->commands;
	add_command(cmds, split[0], op[0]);
	current = get_last_command(*cmds);
	i = 1;
	while (split[i] != NULL)
	{
		if (!op)
			return (1);
		if (op[i] == 0)
			break;
		if (op[i] == 2 || op[i] == 1 || op[i] == 3)
		{
			add_command(cmds, split[i], op[i]);
			current = get_last_command(*cmds);
		}
		else if (op[i] != 0)
		{
			buffer = add_io(&current->redirection, split[i], op[i], mini);
			add_buff_to_last(&current, buffer);
		}
		i++;
	}
	return (0);
}

int	*get_op_loop(char *s, int *i, int *output)
{
	while (s[i[0]] != '\0')
	{
		if (s[i[0]] == '"' && i[3] == 0)
		{
			i[2] = !i[2];
			i[0]++;
		}
		else if (s[i[0]] == '\'' && i[2] == 0)
		{
			i[3] = !i[3];
			i[0]++;
		}
		else if (cmp(s[i[0]]) && i[2] == 0 && i[3] == 0)
		{
			if (!handle_operator(s, &i[0], &i[1], output))
			{
				free(output);
				return (NULL);
			}
		}
		else
			i[0]++;
	}
	return (output);
}

int	*get_operators(char *s)
{
	int	i[4];
	int	*output;

	i[0] = 0;
	i[1] = 1;
	i[2] = 0;
	i[3] = 0;
	output = malloc((splitlen(s, ' ') + 1) * sizeof(int));
	output[0] = 2;
	if (!output)
		return (NULL);
	output = get_op_loop(s, i, output);
	output[i[1]] = '\0';
	return (output);
}

void print_commands(t_commands *commands)
{
	t_commands *current = commands;
	int i = 1;
	while (current)
	{
		printf("Command: %s\n", current->command[0]);
		while (current->command[i] != NULL)
		{
			printf("  Arg[%d]: %s\n", i, current->command[i]);
			i++;
		}
        t_io_red *red = current->redirection;
        while (red)
        {
            printf("  Redirection: type=%d, file=%s\n", red->in_or_out, red->file);
            red = red->next;
        }
        current = current->next;
    }
}

void	print_redirection(t_io_red *redirection)
{
	t_io_red	*current;

	current = redirection;
	while (current != NULL)
	{
		printf("IO Type: %d\n", current->in_or_out);
		printf("File: %s\n", current->file);
		current = current->next;
	}
}
