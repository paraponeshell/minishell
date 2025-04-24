/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_apply_redirection.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:14:09 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 12:36:26 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	fd_print_redirection(int fd)
{
	if (fd < 0)
	{
		ft_putendl_fd("Error: No such file or directory", 2);
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		ft_putendl_fd("Error: dup2 failed", 2);
		close(fd);
		exit(1);
	}
	close(fd);
	return (1);
}

int	heredoc_redirection(int *input_fd, t_io_red **current, t_env *env)
{
	pipe(input_fd);
	get_heredoc(input_fd, ft_strdup((*current)->file), env);
	close(input_fd[1]);
	dup2(input_fd[0], STDIN_FILENO);
	return (1);
}

void	dup2_and_close(int i_fd, int o_fd, int i, int o)
{
	if (i == 0 && i_fd != 0)
	{
		if (dup2(i_fd, STDIN_FILENO) == -1)
			exit(print_error_exit("Error: dup2 failed"));
		close(i_fd);
	}
	if (o == 0 && o_fd != 1)
	{
		if (dup2(o_fd, STDOUT_FILENO) == -1)
			exit(print_error_exit("Error: dup2 failed"));
		close(o_fd);
	}
}

int	apply_redirection_6_and_7(t_io_red **current)
{
	int	fd;

	if ((*current)->in_or_out == 7)
	{
		if (access((*current)->file, F_OK) == 0 && access((*current)->file, W_OK) == -1)
			exit(print_error_exit("Error: Permission denied"));
		fd = open((*current)->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
			exit(print_error_exit("Error: No such file or directory"));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if ((*current)->in_or_out == 6)
	{
		if (access((*current)->file, F_OK) == 0 && access((*current)->file, W_OK) == -1)
			exit(print_error_exit("Error: Permission denied"));
		fd = open((*current)->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			exit(print_error_exit("Error: No such file or directory"));
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	return (1);
}

void	apply_redirection(t_io_red *redirections, int i_fd, int o_fd,
		t_env *env)
{
	t_io_red	*current;
	int			o;
	int			i;
	int			input_fd[2];
	int			fd;

	o = 0;
	i = 0;
	current = redirections;
	while (current != NULL)
	{
		if (current->in_or_out == 5)
		{
			fd = open(current->file, O_RDONLY);
			i = i + fd_print_redirection(fd);
		}
		else if (current->in_or_out == 4)
			i = i + heredoc_redirection(input_fd, &current, env);
		else if ((current->in_or_out == 7) || (current->in_or_out == 6))
			o = o + apply_redirection_6_and_7(&current);
		current = current->next;
	}
	dup2_and_close(i_fd, o_fd, i, o);
	return ;
}
