/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:41:57 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 16:35:03 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_only(t_env *env)
{
	int		size;
	char	*var;

	var = ft_getallenv(env, "#");
	size = ft_atoi(var);
	return (size);
}

int	first_not_null(t_commands *t)
{
	int	i;

	i = 0;
	if (t->command[i] != NULL && t->command[i][0] == '\0'
		&& t->command[i] != NULL)
		while (t->command[i] != NULL && t->command[i][0] == '\0')
			i++;
	return (i);
}

int	print_error_exit(char *str)
{
	ft_putendl_fd(str, 2);
	return (1);
}

int	if_is_only_env_comm(t_commands *t, t_inout_var var, int p_fd, t_env *env)
{
	int	status;

	status = 0;
	if (is_only(env) == 1)
		status = commandbuiltin(t, var, p_fd, env);
	return (status);
}
