/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utilities_3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:01:14 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 15:02:39 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	putlist(t_mini *mini, char **split, int *op)
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
			break ;
		if (op[i] == 2 || op[i] == 1 || op[i] == 3)
		{
			add_command(cmds, split[i], op[i]);
			current = get_last_command(*cmds);
		}
		else if (op[i] != 0)
			add_buff_to_last(&current, add_io(&current->redirection, split[i],
					op[i], mini));
		i++;
	}
	return (0);
}
