/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   relative_execute.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:46:46 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 15:36:46 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	double_free(char *a, char *b)
{
	free(a);
	free(b);
}

char	*nepasprint_free(char *s)
{
	free(s);
	return (ft_strdup("g%DH1211DVJQbaz"));
}

char	*ft_relative_executable(char *str)
{
	char	*result;
	int		i;

	i = ft_strlen(str);
	while (--i >= 0)
	{
		if (str[i] == '/')
			break ;
	}
	result = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result = ft_substr(str, i + 1, ft_strlen(str));
	return (result);
}

char	*ft_relative_path_string(char *str)
{
	char	*result;
	int		i;

	i = ft_strlen(str);
	while (--i >= 0)
	{
		if (str[i] == '/')
			break ;
	}
	result = malloc((i + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result = ft_substr(str, 0, i);
	return (result);
}

char	*ft_relative_path(char *str)
{
	char	current_path[1024];
	char	*relative_path;
	char	*result;
	char	*executable;

	getcwd(current_path, sizeof(current_path));
	relative_path = ft_relative_path_string(str);
	executable = ft_relative_executable(str);
	result = ft_strjoin(current_path, "/");
	result = ft_strjoin(result, relative_path);
	result = ft_strjoin(result, "/");
	result = ft_strjoin(result, executable);
	return (result);
}
