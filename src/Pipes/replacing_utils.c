/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 15:59:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/26 11:32:14 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	srch_dollar(char c)
{
	if (c == '$')
		return (1);
	else
		return (0);
}

char	*clean_env(char *str)
{
	int		start;
	int		end;
	char	*trimmed;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && str[start] == ' ')
		start++;
	end = ft_strlen(str) - 1;
	while (end > start && str[end] == ' ')
		end--;
	trimmed = ft_substr(str, start, end - start + 1);
	return (trimmed);
}

char	*ft_getenv(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	if (ft_strcmp(name, "&") == 0)
		return (NULL);
	else if (ft_strcmp(name, "+") == 0)
		return (NULL);
	while (temp != NULL)
	{
		if (ft_strcmp(temp->value, name) == 0)
			return (temp->result);
		temp = temp->next;
	}
	return (NULL);
}

char	*ft_getallenv(t_env *env, char *name)
{
	t_env	*temp;

	temp = env;
	while (temp != NULL)
	{
		if (ft_strcmp(temp->value, name) == 0)
			return (temp->result);
		temp = temp->next;
	}
	return (NULL);
}

int	print_file_error(char *args)
{
	int	status;

	ft_putstr_fd(args, 2);
	ft_putendl_fd(": file not found || permission denied || is a directory", 2);
	status = 1;
	return (status);
}
