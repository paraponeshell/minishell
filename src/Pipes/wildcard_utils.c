/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 15:47:42 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 15:51:45 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*returned_value_wildcard(char *str, char *pattern, char *temp)
{
	if (temp)
		str = ft_replacesubstr(str, pattern, temp);
	else
		str = ft_strdup(str);
	free(temp);
	return (str);
}

char	*insert_files(char *pattern, char *str)
{
	char	**filenames;
	char	*temp;
	int		k;

	filenames = get_filenames();
	k = 0;
	temp = NULL;
	while (filenames[k] != NULL)
	{
		if (pattern_matching(pattern, filenames[k]))
		{
			if (!temp)
				temp = ft_strdup(filenames[k]);
			else
			{
				temp = ft_strjoinfree(temp, " ");
				temp = ft_strjoinfree(temp, filenames[k]);
			}
		}
		k++;
	}
	free_split(filenames);
	return (returned_value_wildcard(str, pattern, temp));
}

int	pattern_matching(char *pattern, char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (1);
	if (*pattern == '*' && *(pattern + 1) != '\0' && *filename == '\0')
		return (0);
	if (*pattern == *filename)
		return (pattern_matching(pattern + 1, filename + 1));
	if (*pattern == '*')
	{
		return (pattern_matching(pattern + 1, filename)
			|| pattern_matching(pattern, filename + 1));
	}
	return (0);
}
