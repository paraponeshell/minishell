/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities_bordel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:24:03 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 02:00:21 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	handle_quotes_bis(char *s, int i)
{
	char	quote;

	quote = s[i];
	i++;
	while (s[i] != quote && s[i] != '\0')
		i++;
	if (s[i] == quote)
		i++;
	return (i);
}

int	handle_operator(char *s, int *i, int *j, int *output)
{
	int	op;

	op = find_op(&s[*i]);
	if (op == 0)
		return (0);
	output[*j] = op;
	(*j)++;
	if (op == 1 || op == 3 || op == 4 || op == 6)
		*i += 2;
	else
		*i += 1;
	return (1);
}

void	free_cmd_red(t_commands **cmd, t_io_red **red)
{
	free_cmd(cmd);
	free_red(red);
}

void	init_list(char *list)
{
	list[0] = '|';
	list[1] = '<';
	list[2] = '>';
	list[3] = '&';
	list[4] = '\0';
}

void	close_pipes(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}
