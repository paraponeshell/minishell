/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:42:58 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 15:48:26 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	srchr_wildcard(char *str)
{
	int	i;

	i = 0;
	if (str == NULL)
		return (0);
	if (str[0] == '\0')
		return (0);
	if (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] != ' ')
		{
			if (str[i] == '*')
			{
				return (1);
			}
			i++;
		}
	}
	return (0);
}

char	*ft_strjoinfree(char *s1, char *s2)
{
	char	*str;

	str = ft_strjoin(s1, s2);
	free(s1);
	return (str);
}

int	find_length(int i, int k, char *str, char *output)
{
	int	j;

	if (!output)
		return (0);
	if (str[k] != '\0')
	{
		j = ft_strchrpos(output, str[k]);
		return (j - 1);
	}
	else
		return (i);
}

char	*write_pattern(char *str, char *pattern, int *i, int j)
{
	while (i[1] < i[0])
	{
		pattern[i[1] - j] = str[i[1]];
		i[1]++;
	}
	pattern[i[1] - j] = '\0';
	return (pattern);
}

char	*handle_wildcard(char *str, t_var_env_bundle *var)
{
	char	*output;
	char	*pattern;
	int		i[2];

	i[0] = var->j;
	while (str[i[0]] != ' ' && str[i[0]] != '\0'
		&& str[i[0]] != '"' && str[i[0]] != '\'')
		i[0]++;
	pattern = malloc((i[0] - var->j + 1) * sizeof(char));
	i[1] = var->j;
	pattern = write_pattern(str, pattern, i, var->j);
	if (ft_strchr(pattern, '/') == 0)
		output = insert_files(pattern, str);
	else if (ft_strcmp(str, pattern) == 0)
		output = ft_replacesubstr(str, pattern, " ");
	else
		output = ft_strtrim(str, pattern);
	var->j = find_length(var->j, i[0], str, output);
	free(str);
	free(pattern);
	return (output);
}
