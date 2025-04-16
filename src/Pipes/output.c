/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:06:06 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 16:28:16 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	write_output(int buff_fd, t_io_red *redirection)
{
	t_io_red	*temp;
	int			output_fd;

	output_fd = 1;
	temp = redirection;
	while (temp != NULL)
	{
		if (temp->in_or_out == 7)
		{
			if (output_fd != 1)
				close(output_fd);
			output_fd = open(temp->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (temp->in_or_out == 6)
		{
			if (output_fd != 1)
				close(output_fd);
			output_fd = open(temp->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		}
		temp = temp->next;
	}
	copy_single(buff_fd, output_fd);
	if (output_fd != 1)
		close(output_fd);
	if (buff_fd != 0)
		close(buff_fd);
}

void	copy(int buff_fd, int *o_fd, int size)
{
	char	*line;
	int		i;

	if (!buff_fd)
	{
		line = malloc (1 * sizeof(char));
		line[0] = '\0';
	}
	else
		line = get_next_line(buff_fd);
	if (line == NULL)
		printf("erreur gnl");
	while (line != NULL)
	{
		i = 0;
		while (size > i)
		{
			write(o_fd[i++], line, ft_strlen(line));
		}
		free(line);
		line = get_next_line(buff_fd);
	}
}

void	copy_single(int buff_fd, int o_fd)
{
	char	*line;

	if (buff_fd == 0)
	{
		write(o_fd, "\0", 1);
	}
	else
	{
		line = get_next_line(buff_fd);
		while (line != NULL)
		{
			write(o_fd, line, ft_strlen(line));
			free(line);
			line = get_next_line(buff_fd);
		}
	}
}

int	count_output_redirections(t_io_red *redirection)
{
	int	count;

	count = 0;
	while (redirection != NULL)
	{
		if (redirection->in_or_out == 7 || redirection->in_or_out == 6)
			count++;
		redirection = redirection->next;
	}
	return (count);
}
