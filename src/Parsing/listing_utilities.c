/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:50:05 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 13:20:36 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_commands	*init_command_node(char **command)
{
	t_commands	*node;

	node = malloc(sizeof(t_commands));
	node->pipe_type = 2;
	node->command = command;
	node->redirection = NULL;
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
