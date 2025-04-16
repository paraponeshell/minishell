/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   first_split_utilities.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:12:23 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 14:41:27 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	assign_start_value(t_var_bundle *var)
{
	var->i = 0;
	var->j = 0;
	var->y = 0;
	var->s_quotes = 0;
	var->d_quotes = 0;
}

char	**perror_and_free(char **output, int size)
{
	int	i;

	i = 0;
	printf("Error: unmatched quotes\n");
	while (i < size)
	{
		free(output[i]);
		i++;
	}
	free(output);
	return (NULL);
}

char	*crop_str(char *s, int start, int end)
{
	char	*output;

	printf("start = %d, end = %d\n", start, end);
	output = malloc((end - start + 1) * sizeof(char));
	ft_strncpy(output, s + start, (end - start));
	output[end - start] = '\0';
	return (output);
}
