/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:20:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 16:32:52 by aharder          ###   ########.fr       */
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
		print_split(splitted);
		free_split(splitted);
		free(operator);
		printf("Error: ambiguous redirection\n");
		return ;
	}
	free_split(splitted);
	free(operator);
	add_red_to_env(&mini->redirection, &mini->env);
	add_cmd_to_env(&mini->commands, &mini->env);
	//print_commands(mini->commands);
	//print_redirection(mini->redirection);
	//if (valid_line(mini->commands, mini->redirection) == 0)
		createpipes(mini->commands, mini->redirection, mini->env);
	while (wait(&exit_status) > 0)
		add_exit_status(exit_status, &mini->env);
	free_cmd_red(&mini->commands, &mini->redirection);
}

int	putlist(t_mini	*mini, char **split, int *op)
{
	char		*buffer;
	t_commands	**cmds;
	t_io_red	**red;
	int			i;

	cmds = &mini->commands;
	red = &mini->redirection;
	i = 0;
	while (split[i] != NULL)
	{
		if (!op)
			return (1);
		if (op[i] == 2 || op[i] == 1 || op[i] == 3)
			add_command(cmds, split[i], op[i]);
		else if (op[i] != 0)
		{
			buffer = add_io(red, split[i], op[i], mini);
			if (buffer == NULL)
			{
				//continue;
				free_cmd_red(cmds, red);
				return (1);
			}
			add_buff_to_last(cmds, buffer);
		}
		i++;
	}
	return (0);
}

int	*get_operators(char *s)
{
	int	i[4];
	int	*output;

	i[0] = 0;
	i[1] = 1;
	i[2] = 0;
	i[3] = first_split_size(s);
	output = malloc((i[3] + 1) * sizeof(int));
	if (!output)
		return (NULL);
	output[0] = 2;
	while (s[i[0]] != '\0')
	{
		if ((cmp(s[i[0]]) == 1) && !i[2])
			handle_operator(s, &i[0], &i[1], output);
		else if (s[i[0]] == '"')
			i[2] = !i[2];
		i[0]++;
		if (i[0] >= ft_strlen(s))
		{
			free(output);
			return (NULL);
		}
		if (i[1] >= i[3])
			return (output);
	}
	output[i[1]] = '\0';
	return (output);
}

void	print_commands(t_commands *commands)
{
	t_commands	*current;
	int			i;

	i = 0;
	current = commands;
	while (current != NULL)
	{
		printf("Pipe Type: %d\n", current->pipe_type);
		while (current->command[i] != NULL)
		{
			printf("Command[%d]: %s\n", i, current->command[i]);
			i++;
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
