/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:25:45 by aharder           #+#    #+#             */
/*   Updated: 2025/04/17 16:13:36 by jmeli            ###   ########.fr       */
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

void	display_value_and_result(char *str, char *str2)
{
	ft_putstr_fd(str, 1);
	ft_putchar_fd('=', 1);
	ft_putendl_fd(str2, 1);
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
				display_value_and_result(ptr->value, ptr->result);
		}
		ptr = ptr->next;
	}
	return (0);
}
