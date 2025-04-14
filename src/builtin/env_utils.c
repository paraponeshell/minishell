/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:15:22 by aharder           #+#    #+#             */
/*   Updated: 2025/04/14 16:07:38 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sort_env_alphabetically(char **array)
{
	int	i;
	int	j;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		j = i + 1;
		while (array[j])
		{
			if (ft_strcmp(array[i], array[j]) > 0)
				swap_strings(&array[i], &array[j]);
			j++;
		}
		i++;
	}
}

char	*env_value_and_result(t_env **ptr)
{
	char	*temp;
	char	*temp_2;
	char	*string;

	temp = ft_strjoin((*ptr)->value, "=\"");
	if ((*ptr)->result)
	{
		temp_2 = ft_strjoin((*ptr)->result, "\"");
		string = ft_strjoin(temp, temp_2);
		free(temp_2);
	}
	else
		string = ft_strjoin((*ptr)->value, "");
	free(temp);
	return (string);
}

char	**copy_environ(t_env **env)
{
	char	**array;
	t_env	*ptr;
	int		i;

	array = (char **)malloc(sizeof(*array) * (ft_list_size(*env) + 1));
	if (array == NULL)
		return (NULL);
	ptr = *env;
	i = 0;
	while (ptr)
	{
		array[i] = env_value_and_result(&ptr);
		if (!array[i])
			free_array(array, i);
		ptr = ptr->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

int	index_existing_var(char *arg, t_env **env)
{
	int		len;
	int		i;
	t_env	*ptr;

	if (ft_strchr(arg, '='))
	{
		len = 0;
		while (arg[len] && (arg[len] != '='))
			len++;
	}
	else
		len = ft_strlen(arg);
	i = 0;
	ptr = *env;
	while (ptr)
	{
		if (ptr->value && ft_strncmp(ptr->value, arg, len) == 0)
			return (i);
		i++;
		ptr = ptr->next;
	}
	return (-1);
}

int	equal_pos(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '=')
		i++;
	return (i);
}
