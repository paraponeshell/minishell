/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:25:45 by aharder           #+#    #+#             */
/*   Updated: 2025/04/14 16:14:06 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	display_error(char *arg)
{
	ft_putstr_fd("env: \'", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("\': No such file or directory", 2);
	return (1);
}

int	env(t_env **env, char **cmd)
{
	t_env	*ptr;
	int		i;

	ptr = *env;
	i = 0;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], "env") != 0)
		{
			exit(display_error(cmd[i]));
		}
		i++;
	}
	while (ptr)
	{
		if (ptr->result)
		{
			if ((strcmp(ptr->value, "?") != 0) && (strcmp(ptr->value, "&") != 0)
				&& (strcmp(ptr->value, "+") != 0) && (strcmp(ptr->result,
						"''") != 0))
				printf("%s=%s\n", ptr->value, ptr->result);
		}
		ptr = ptr->next;
	}
	return (0);
}
