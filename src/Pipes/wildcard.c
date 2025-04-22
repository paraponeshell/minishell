/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 18:42:58 by aharder           #+#    #+#             */
/*   Updated: 2025/04/22 13:58:30 by aharder          ###   ########.fr       */
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

char	*handle_wildcard(char *str, t_var_env_bundle *var)
{
	char	*output;
	char	*pattern;
	int		i[2];

	i[0] = var->j;
	printf("var d_quotes : %d\n var s_quotes : %d\n", var->d_quotes, var->s_quotes);
	while (str[i[0]] != ' ' && str[i[0]] != '\0'
		&& str[i[0]] != '"' && str[i[0]] != '\'')
		i[0]++;
	pattern = malloc((i[0] - var->j + 1) * sizeof(char));
	i[1] = var->j;
	while (i[1] < i[0])
	{
		pattern[i[1] - var->j] = str[i[1]];
		i[1]++;
	}
	pattern[i[1] - var->j] = '\0';
	if (ft_strchr(pattern, '/') == 0)
		output = insert_files(pattern, str);
	else if (ft_strcmp(str, pattern) == 0)
		output = ft_replacesubstr(str, pattern, " ");
	else
	{
		output = ft_strtrim(str, pattern);
		//	var->j = i[0];
	}
	var->j = find_length(var->j, i[0], str, output);
	free(str);
	free(pattern);
	return (output);
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
	if (temp)
		str = ft_replacesubstr(str, pattern, temp);
	else
		str = ft_strdup(str);
	free(temp);
	free_split(filenames);
	return (str);
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
