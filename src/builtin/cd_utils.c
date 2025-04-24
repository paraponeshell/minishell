/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 17:42:46 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 12:16:47 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*return_pwd_ptr_result(t_env **env)
{
	t_env	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (ft_strncmp(ptr->value, "PWD", 3) == 0 && ptr->result)
			return (ptr->result);
		ptr = ptr->next;
	}
	return (NULL);
}

void	update_old_with_current(t_env **env)
{
	char	*current_pwd;

	current_pwd = ft_strdup(return_pwd_ptr_result(env));
	update_old_pwd(env, current_pwd);
	free(current_pwd);
}

int	home_is_set_but_no_value(t_env **env)
{
	t_env	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (ft_strncmp(ptr->value, "HOME", 4) == 0 && ptr->result)
			return (1);
		ptr = ptr->next;
	}
	return (0);
}
