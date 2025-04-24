/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:20:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 14:25:09 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		count_dquotes_and_squotes(char *str, int i, int *d_quotes, int *s_quotes)
{
	if (str[i] == '"')
		*d_quotes = !*d_quotes;
	else if (str[i] == '\'')
		*s_quotes = !*s_quotes;
	if (*d_quotes == 0 && *s_quotes == 0)
		return (1);
	return (0);
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
		/*
		if (str[i] == '"')
			d_quotes = !d_quotes;
		else if (str[i] == '\'')
			s_quotes = !s_quotes;			
		if (d_quotes == 0 && s_quotes == 0)
		*/
		if (count_dquotes_and_squotes(str, i, &d_quotes, &s_quotes))
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

void	parser_2(t_mini *mini)
{
	int			exit_status;
	
	add_red_to_env(&mini->redirection, &mini->env);
	add_cmd_to_env(&mini->commands, &mini->env);
		createpipes(mini->commands, mini->redirection, mini->env);
	block_signal(SIGQUIT);
	while (wait(&exit_status) > 0)
		add_exit_status(exit_status, &mini->env);
	free_cmd_red(&mini->commands, &mini->redirection);
}

void	parser(char *str, t_mini *mini)
{
	char		**splitted;
	int			*operator;

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
		free_split(splitted);
		free(operator);
		printf("Error: ambiguous redirectiond\n");
		return ;
	}
	free_split(splitted);
	free(operator);
	parser_2(mini);
}

int	putlist(t_mini	*mini, char **split, int *op)
{
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
			add_buff_to_last(&current, add_io(&current->redirection, split[i], op[i], mini));
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


