/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing_utilities_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:29:36 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 13:30:15 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_skip_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	return (i);
}

int	ft_skip_non_spaces(char *str, int i)
{
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

char	*rm_first_word(char *str)
{
	int		i;
	int		k;
	char	*output;
	char	quote;

	i = ft_skip_only_spaces(str);
	k = 0;
	if (str[i] == '"' || str[i] == '\'')
	{
		quote = str[i++];
		while (str[i] && str[i] != quote)
			i++;
		if (str[i] == quote)
			i++;
	}
	else
		i = ft_skip_non_spaces(str, i);
	output = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	if (!output)
		return (NULL);
	while (str[i] != '\0')
		output[k++] = str[i++];
	output[k] = '\0';
	return (output);
}
