/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing_utilities_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:16:41 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 13:17:30 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	return (i);
}

char	*ft_word(char *str, int i, int j)
{
	char	*word;

	word = malloc(j - i + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, &str[i], j - i + 1);
	return (word);
}

char	*first_word(char *str)
{
	int		j;
	char	*word;
	char	quote;

	j = ft_skip_spaces(str);
	if (str[j] == '\"' || str[j] == '\'')
	{
		quote = str[j];
		j++;
		while (str[j] && str[j] != quote)
			j++;
		if (str[j] != quote)
		{
			ft_putendl_fd("Error: unmatched quote", 2);
			return (NULL);
		}
		j++;
	}
	else
	{
		while (str[j] && str[j] != ' ')
			j++;
	}
	word = ft_word(str, ft_skip_spaces(str), j);
	return (word);
}
