/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:50:05 by aharder           #+#    #+#             */
/*   Updated: 2025/04/18 16:03:59 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_commands	*init_command_node(char **command)
{
	t_commands	*node;

	node = malloc(sizeof(t_commands));
	node->pipe_type = 2;
	node->command = command;
	node->next = NULL;
	return (node);
}

t_commands	*get_last_command(t_commands *a)
{
	while (a->next != NULL)
		a = a->next;
	return (a);
}

char	**merge_command(char **old, char **to_add)
{
	int		i;
	int		j;
	int		old_size;
	char	**output;

	old_size = array_size(old);
	output = malloc((old_size + array_size(to_add) + 1) * sizeof(char *));
	i = 0;
	while (old[i] != NULL)
	{
		output[i] = ft_strdup(old[i]);
		i++;
	}
	j = 0;
	while (to_add[j] != NULL)
		output[i++] = ft_strdup(to_add[j++]);
	output[i] = NULL;
	return (output);
}

void	add_buff_to_last(t_commands **a, char *str)
{
	t_commands	*last;
	char		**buffer_split;
	char		**new_command;


	if (!str || str[0] == '\0')
	{
		free(str);
		return ;
	}
	buffer_split = second_split(str, ' ');
	if (!*a)
		*a = init_command_node(buffer_split);
	else
	{
		last = get_last_command(*a);
		new_command = merge_command(last->command, buffer_split);
		free_split(last->command);
		free_split(buffer_split);
		free(str);
		last->command = new_command;
	}
}

char *first_word(char *str)
{
    int i = 0;
    int j = 0;

    // Ignore les espaces au début
    while (str[i] && str[i] == ' ')
        i++;

    // Trouve la fin du premier mot
    j = i;
    while (str[j] && str[j] != ' ')
        j++;

    // Alloue de la mémoire pour le premier mot
    char *word = malloc(j - i + 1); // +1 pour le caractère nul
    if (!word)
        return (NULL);

    // Copie le premier mot dans la nouvelle chaîne
    ft_strlcpy(word, &str[i], j - i + 1); // Copie avec le caractère nul
    return (word);
}
