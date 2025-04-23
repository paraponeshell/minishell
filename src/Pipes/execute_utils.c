/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:41:57 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 14:42:34 by jmeli            ###   ########.fr       */
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
