/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:17:41 by aharder           #+#    #+#             */
/*   Updated: 2025/04/23 09:40:23 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	env_bundle_init(t_var_env_bundle *var, int size)
{
	var->i = 0;
	var->j = 0;
	var->k = 0;
	var->d_quotes = 0;
	var->s_quotes = 0;
	var->size = size;
}

void	check_env(char **temp, t_env *env, int size)
{
	t_var_env_bundle	var;
	int					k;

	env_bundle_init(&var, size);
	while (var.i < size)
	{
		var.j = 0;
		while (temp[var.i][var.j] != '\0')
		{
			k = srch_dollar(temp[var.i][var.j]);
			while ((k == 0 || var.s_quotes) && temp[var.i][var.j] != '\0')
			{
				temp[var.i] = handle_env_quotes(temp[var.i], var.j, &var, temp);
				//printf("var d_quotes : %d\n var s_quotes : %d\n char : %c\n var.j : %d", var.d_quotes, var.s_quotes, temp[var.i][var.j], var.j);
				/*if (!var.s_quotes && !var.d_quotes
						&& var.j >= 0 && srchr_wildcard(&temp[var.i][var.j]))
					temp[var.i] = handle_wildcard(temp[var.i], &var);*/
				var.j++;
				k = srch_dollar(temp[var.i][var.j]);
			}
			var.k = env_size(temp[var.i], var, env);
			if (temp[var.i][var.j] != '\0')
				temp[var.i] = replace(temp[var.i], &var, env);
			var.j += var.k;
			if (var.j > ft_strlen(temp[var.i]))
				break ;
		}
		var.i++;
	}
}

int is_there_an_export(char **temp, t_var_env_bundle *var)
{
    int i;

    i = 0;
    if (temp == NULL)
        return (0);
    while (i < var->size)
	{
        if (temp[i] && ft_strncmp(temp[i], "export", 6) == 0)
            return (1);
        i++;
    }
    return (0);
}

char	*handle_env_quotes(char *str, int i, t_var_env_bundle *var, char **temp)
{
	int		disable_wildcard;

	if (is_there_an_export(temp, var))
		disable_wildcard = 1;
	else
		disable_wildcard = 0;
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
	else if (!var->d_quotes && !var->s_quotes && !disable_wildcard && srchr_wildcard(&str[var->j]))
		str = handle_wildcard(str, var);
	return (str);
}

char	*replace(char *s, t_var_env_bundle *v, t_env *env)
{
	char	*prefix;
	char	*suffix;
	char	*var;
	char	*value;
	char	*buffer;
	int		i;

	i = v->j;
	prefix = ft_substr(s, 0, i);
	var = ft_substr(s, i + 1, var_size(s, i + 1));
	suffix = ft_substr(s, i + 1 + var_size(s, i + 1), ft_strlen(s) - i + 1 + var_size(s, i + 1));
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
			buffer = ft_getenv(env, var);
			if (buffer)
				value = ft_strdup(ft_getenv(env, var));
			else
				value = ft_strdup("");
		}
		else
			value = clean_env(ft_getenv(env, var));
	}
	free(var);
	if (!value)
	{
		free(s);
		s = ft_strjoin(prefix, suffix);
		free(prefix);
		free(suffix);
		return (s);
	}
	free(s);
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

int	ft_strchrpos(char *str, int searchedChar)
{
	int	i;

	i = 0;
	while (*str)
	{
		if (*str == (char)searchedChar)
			return (i);
		str++;
		i++;
	}
	if (searchedChar == '\0')
		return (0);
	return (0);
}
