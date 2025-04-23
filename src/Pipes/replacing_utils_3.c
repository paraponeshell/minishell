/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacing_utils_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:47:13 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/23 09:51:35 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
