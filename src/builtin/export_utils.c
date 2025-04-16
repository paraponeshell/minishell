/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:11:46 by aharder           #+#    #+#             */
/*   Updated: 2025/04/15 15:11:11 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_array(char **array, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	ft_list_size(t_env *begin_list)
{
	int		size;
	t_env	*current;

	size = 0;
	current = begin_list;
	while (current != NULL)
	{
		size++;
		current = current->next;
	}
	return (size);
}

void	ft_env_push_back(t_env **begin_list, char *data)
{
	t_env	*new_elem;
	t_env	*current;

	new_elem = ft_create_var(data);
	if (*begin_list == NULL)
	{
		*begin_list = new_elem;
	}
	else
	{
		current = *begin_list;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_elem;
	}
}

int	update(char *arg, int index, t_env **env)
{
	int		i;
	t_env	*ptr;

	i = 0;
	ptr = *env;
	while (i < index)
	{
		ptr = ptr->next;
		i++;
	}
	if (ft_strchr(arg, '='))
	{
		free(ptr->value);
		free(ptr->result);
		ptr->value = ft_substr(arg, 0, equal_pos(arg));
		ptr->result = ptr_result(arg);
	}
	return (0);
}

int	create_or_update_var(char *arg, t_env **env)
{
	int	index;

	index = -1;
	index = index_existing_var(arg, env);
	if (index >= 0)
		update(arg, index, env);
	else
		ft_env_push_back(env, arg);
	return (0);
}
