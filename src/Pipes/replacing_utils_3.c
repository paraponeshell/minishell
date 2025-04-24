/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:47:13 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 10:38:32 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_disable_wildcard(t_var_env_bundle *var, char **temp)
{
	int	disable_wildcard;

	if (is_there_an_export(temp, var))
		disable_wildcard = 1;
	else
		disable_wildcard = 0;
	return (disable_wildcard);
}

int	is_there_an_export(char **temp, t_var_env_bundle *var)
{
	int	i;

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
