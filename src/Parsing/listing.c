/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:27:22 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 12:49:12 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_command(t_commands **a, char *splitted, int type)
{
	t_commands	*buffer;
	t_commands	*last;

	buffer = malloc(sizeof(t_commands));
	buffer->pipe_type = type;
	buffer->command = second_split(splitted, ' ');
	buffer->next = NULL;
	if (!*a)
		*a = buffer;
	else
	{
		last = *a;
		while (last->next)
			last = last->next;
		last->next = buffer;
	}
}

char	*free_and_null(t_io_red *buffer)
{
	free(buffer->file);
	free(buffer);
	return (NULL);
}

int	check_redirection(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
			return (0);
		i++;
	}
	return (1);
}

char	*add_io(t_io_red **a, char *splitted, int type, t_mini *mini)
{
	t_io_red	*buffer;
	t_io_red	*last;
	char		*output;

	buffer = malloc(sizeof(t_io_red));
	buffer->in_or_out = type;
	buffer->file = first_word(splitted);
	if (buffer->file == NULL)
		return (free_and_null(buffer));
	if (type != 4)
		check_env(&buffer->file, mini->env, 1);
	if (check_redirection(buffer->file) == 0)
		return (free_and_null(buffer));
	buffer->next = NULL;
	output = rm_first_word(splitted);
	if (!*a)
		*a = buffer;
	else
	{
		last = *a;
		while (last->next)
			last = last->next;
		last->next = buffer;
	}
	return (output);
}

char	*rm_first_word(char *str)
{
	int		i;
	int		k;
	char	*output;

	i = 0;
	k = 0;
	while (str[i] == ' ')
		i++;
	while (str[i] != ' ' && str[i] != '\0')
		i++;
	output = malloc((ft_strlen(str) - i + 1) * sizeof(char));
	while (str[i] != '\0')
		output[k++] = str[i++];
	output[k] = '\0';
	return (output);
}
