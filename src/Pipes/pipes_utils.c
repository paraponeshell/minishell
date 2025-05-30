/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 10:41:46 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/26 01:33:50 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	do_i_close(int i, int j, t_commands *t)
{
	if (t->next != NULL)
		close(i);
	if (j)
		close(j);
}

int	iec(char *str)
{
	char	*commands[3];
	int		i;

	i = 0;
	commands[0] = "echo";
	commands[1] = "env";
	commands[2] = "pwd";
	while (i < 3)
	{
		if (ft_strcmp(str, commands[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	is_other_command(char *str)
{
	char	*commands[4];
	int		i;

	i = 0;
	commands[0] = "export";
	commands[1] = "unset";
	commands[2] = "cd";
	commands[3] = "exit";
	while (i < 4)
	{
		if (ft_strcmp(str, commands[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	init_pipes(int p_fd[2], int b_fd[2])
{
	(void)b_fd;
	(void)p_fd;
}
