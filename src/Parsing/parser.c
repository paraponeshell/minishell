/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:20:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 15:05:23 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	count_dquotes_and_squotes(char *str, int i, int *d_quotes, int *s_quotes)
{
	if (str[i] == '"')
		*d_quotes = !*d_quotes;
	else if (str[i] == '\'')
		*s_quotes = !*s_quotes;
	if (*d_quotes == 0 && *s_quotes == 0)
		return (1);
	return (0);
}

void	set_at_zero(int *i, int *d_quotes, int *s_quotes)
{
	*i = 0;
	*d_quotes = 0;
	*s_quotes = 0;
}

int	is_op_correct(char *str)
{
	int	i;
	int	d_quotes;
	int	s_quotes;
	int	op;

	set_at_zero(&i, &d_quotes, &s_quotes);
	if (str == NULL)
		return (1);
	while (str[i] != '\0')
	{
		if (count_dquotes_and_squotes(str, i, &d_quotes, &s_quotes))
		{
			op = i;
			while (str[op] != '\0' && (str[op] == '|' || str[op] == '<'
					|| str[op] == '>'))
				op++;
			if (op - i > 2)
				return (1);
			i = i + op;
		}
		if (i++ >= ft_strlen(str))
			return (0);
	}
	return (0);
}

void	parser_2(t_mini *mini)
{
	int	exit_status;

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
	char	**splitted;
	int		*operator;

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
