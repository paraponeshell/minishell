/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:35:24 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 09:40:16 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	env_size(char *s, t_var_env_bundle v, t_env *env)
{
	char	*var;
	char	*value;
	int		size;
	int		i;

	i = v.j;
	if (s[i] != '\0' && (s[i + 1] == ' ' || s[i + 1] == '\0'))
		return (1);
	if (v.d_quotes && s[i + 1] == '"')
		return (1);
	if (s[i] != '\0' && s[i + 1] != '\0' && ft_isalnum(s[i + 1]) == 0)
	{
		return (1);
	}
	if (s[i] != '\0' && is_end_var(s[i + 1]))
		return (0);
	size = var_size(s, i + 1);
	var = ft_substr(s, i + 1, size);
	value = ft_getenv(env, var);
	free(var);
	if (!value)
		return (0);
	size = ft_strlen(value);
	return (size);
}

int	var_size(char *str, int i)
{
	int	size;

	size = 0;
	if (i > ft_strlen(str))
		return (size);
	if (str[i] == '?')
		return (1);
	if (ft_isdigit(str[i]) == 1)
		return (1);
	while (str[i] != '\0')
	{
		if (is_end_var(str[i]))
			break ;
		size++;
		i++;
	}
	return (size);
}

int	var_size2(char *str, int i, t_var_env_bundle *var)
{
	if (str[i] == ' ')
		return (1);
	else if (str[i] == '\0')
		return (1);
	else if (str[i] == '?')
		return (0);
	else if ((var->d_quotes || var->s_quotes) && ft_isalnum(str[i]) == 0)
	{
		return (1);
	}
	else if (var->d_quotes && str[i] == '"')
		return (1);
	else
		return (0);
}

int	is_end_var(char c)
{
	if (ft_isalnum(c))
		return (0);
	else if (c == '_')
		return (0);
	else
		return (1);
}

char	*ft_strrmchar(char *str, int pos)
{
	char	*output;
	int		i;
	int		j;

	i = 0;
	j = 0;
	output = malloc((ft_strlen(str)) * sizeof(char));
	while (str[i] != '\0')
	{
		if (i != pos)
			output[j++] = str[i];
		i++;
	}
	output[j] = '\0';
	free(str);
	return (output);
}
