/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:17:41 by aharder           #+#    #+#             */
/*   Updated: 2025/04/23 10:38:36 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*handle_env_quotes(char *str, int i, t_var_env_bundle *var, char **temp)
{
	int	disable_wildcard;

	disable_wildcard = ft_disable_wildcard(var, temp);
	if (!var->s_quotes && str[i] == '"')
	{
		var->d_quotes = !var->d_quotes;
		str = ft_strrmchar(str, i);
		if (!var->d_quotes && !disable_wildcard && srchr_wildcard(&str[var->j]))
			str = handle_wildcard(str, var);
		var->j--;
	}
	else if (!var->d_quotes && str[i] == '\'')
	{
		var->s_quotes = !var->s_quotes;
		str = ft_strrmchar(str, i);
		if (!var->s_quotes && srchr_wildcard(&str[var->j]))
			str = handle_wildcard(str, var);
		var->j--;
	}
	else if (!var->d_quotes && !var->s_quotes && !disable_wildcard
		&& srchr_wildcard(&str[var->j]))
		str = handle_wildcard(str, var);
	return (str);
}

char	*value_if_var(t_env *env, char *var)
{
	char	*value;
	char	*buffer;

	buffer = ft_getenv(env, var);
	if (buffer)
		value = ft_strdup(ft_getenv(env, var));
	else
		value = ft_strdup("");
	return (value);
}

char	*value_for_replace(char *s, t_var_env_bundle *v, t_env *env)
{
	char	*value;
	char	*var;
	int		i;

	i = v->j;
	var = ft_substr(s, i + 1, var_size(s, i + 1));
	if (var_size2(s, i + 1, v) == 1)
		value = ft_strdup("$");
	else if (var == NULL || var[0] == '\0')
	{
		v->j--;
		value = ft_strdup("");
	}
	else
	{
		if (v->d_quotes)
		{
			value = value_if_var(env, var);
		}
		else
			value = clean_env(ft_getenv(env, var));
	}
	free(var);
	return (value);
}

char	*replace(char *s, t_var_env_bundle *v, t_env *env)
{
	char	*prefix;
	char	*suffix;
	char	*value;
	int		i;

	i = v->j;
	prefix = ft_substr(s, 0, i);
	suffix = ft_substr(s, i + 1 + var_size(s, i + 1), ft_strlen(s) - i + 1
			+ var_size(s, i + 1));
	value = value_for_replace(s, v, env);
	free(s);
	if (!value)
	{
		s = ft_strjoin(prefix, suffix);
		free(prefix);
		free(suffix);
		return (s);
	}
	s = ft_strjoin(prefix, value);
	free(prefix);
	prefix = ft_strjoin(s, suffix);
	free(s);
	free(suffix);
	free(value);
	return (prefix);
}

char	*quote_replace(char *str, int i, t_env *env)
{
	char	*buffer;
	char	*buff2;
	char	*buff3;
	int		j;

	j = i;
	while (str[j] != ' ' && str[j] != '"' && str[j] != '\0')
		j++;
	j = j - i;
	buffer = ft_substr(str, i, j);
	buff3 = ft_getenv(env, &buffer[1]);
	if (!buff3)
	{
		buff2 = str;
		str = ft_replacesubstr(buff2, buffer, "");
		free(buffer);
		free(buff2);
		return (str);
	}
	buff2 = str;
	str = ft_replacesubstr(buff2, buffer, buff3);
	free(buff2);
	free(buffer);
	return (str);
}
