/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 00:16:22 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 12:53:58 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	swap_strings(char **str1, char **str2)
{
	char	*temp;

	temp = *str1;
	*str1 = *str2;
	*str2 = temp;
}

char	*ptr_result(char *arg)
{
	char	*result;
	int		len;

	len = equal_pos(arg) + 1;
	while (arg[len])
	{
		len++;
	}
	len = len - equal_pos(arg);
	result = ft_substr(arg, equal_pos(arg) + 1, len - 1);
	return (result);
}

int	print_export(t_env **env)
{
	char	**temp;
	int		j;

	temp = copy_environ(env);
	sort_env_alphabetically(temp);
	j = 0;
	while (temp[j])
	{
		ft_putstr_fd("declare -x ", 1);
		printf("%s\n", temp[j]);
		j++;
	}
	free_split(temp);
	return (0);
}

t_env	*ft_create_var(char *arg)
{
	t_env	*new_elem;

	new_elem = (t_env *)malloc(sizeof(t_env));
	if (new_elem == NULL)
		return (NULL);
	if (ft_strchr(arg, '='))
	{
		new_elem->value = ft_substr(arg, 0, equal_pos(arg));
		new_elem->result = ptr_result(arg);
		new_elem->next = NULL;
	}
	else
	{
		new_elem->value = ft_strdup(arg);
		new_elem->result = NULL;
		new_elem->next = NULL;
	}
	return (new_elem);
}

int	export(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	while (args[i])
	{
		if (arg_var_is_valid(args[i]) && arg_val_is_valid(args[i])
			&& ft_strcmp(args[i], "") != 0)
		{
			create_or_update_var(args[i], env);
		}
		else
		{
			printf("export: invalid character\n");
			exit_status = 1;
		}
		i++;
	}
	if (i == 1)
		return (print_export(env));
	else
		return (exit_status);
}
