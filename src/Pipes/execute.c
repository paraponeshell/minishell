/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:55:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/26 01:37:57 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_commands *t, t_inout_var var, int p_fd[2], t_env *env)
{
	int	status;

	status = 0;
	if (t->command[var.i] == NULL || t->command[var.i][0] == '\0')
		status = executecommand(t, var, p_fd, env);
	else if (t->command[var.i][0] == '/')
		status = executefullfile(t, env, var, p_fd);
	else if (is_executable(t, var) == 1)
	{
		if (access(t->command[var.i], F_OK | X_OK) == 0)
			status = executefile(t, var, p_fd, env);
		else
			status = print_file_error(t->command[var.i]);
	}
	else if (iec(t->command[var.i]) != -1 || (ft_strcmp(t->command[var.i],
				"export") == 0 && t->command[var.i + 1] == NULL))
		status = executebuiltin(t, var, p_fd, env);
	else if (is_other_command(t->command[var.i]) != -1)
	{
		status = if_is_only_env_comm(t, var, p_fd, env);
	}
	else
		status = executecommand(t, var, p_fd, env);
	return (status);
}

int	executefile(t_commands *command, t_inout_var var, int *o_fd, t_env *env)
{
	extern char	**environ;
	char		*full_cmd;
	pid_t		p;
	int			exit_status;
	int			i_fd;

	i_fd = var.input;
	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		apply_redirection(command->redirection, i_fd, o_fd[1], env);
		if (ft_strncmp(command->command[var.i], "./", 2) == 0
			|| ft_strncmp(command->command[var.i], "../", 3) == 0)
			full_cmd = ft_relative_path(command->command[0]);
		else
			full_cmd = ft_strdup(command->command[var.i]);
		signal(SIGQUIT, handle_signal);
		execve(full_cmd, &command->command[var.i], environ);
		free(full_cmd);
		paranoia_closing();
		exit_fork(env, command);
		exit(1);
	}
	return (exit_status);
}

int	executefullfile(t_commands *commands, t_env *env,
					t_inout_var var, int *o_fd)
{
	int			exit_status;
	pid_t		p;
	extern char	**environ;
	struct stat	sb;

	if (access(commands->command[var.i], F_OK) == -1)
		return (print_file_error(commands->command[var.i]));
	stat(commands->command[var.i], &sb);
	if (S_ISDIR(sb.st_mode))
		return (print_file_error(commands->command[var.i]));
	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		if (!commands->command[var.i])
			exit(1);
		apply_redirection(commands->redirection, var.input, o_fd[1], env);
		signal(SIGQUIT, handle_signal);
		execve(commands->command[var.i], &commands->command[var.i], environ);
		paranoia_closing();
		exit_fork(env, commands);
		exit(1);
	}
	return (exit_status);
}

int	executecommand(t_commands *c, t_inout_var var, int *o_fd, t_env *env)
{
	pid_t		p;
	extern char	**environ;
	char		*full_cmd;
	int			i_fd;

	i_fd = var.input;
	p = fork();
	if (p == 0)
	{
		close(o_fd[0]);
		apply_redirection(c->redirection, i_fd, o_fd[1], env);
		if (c == NULL || c->command == NULL
			|| c->command[var.i] == NULL || c->command[var.i][0] == '\0')
			exit(1);
		full_cmd = get_path(c->command[var.i], env);
		if (full_cmd == NULL)
			exit(1);
		signal(SIGQUIT, handle_signal);
		execve(full_cmd, &c->command[var.i], environ);
		ft_putendl_fd("fail command", 2);
		paranoia_closing();
		exit_fork(env, c);
		exit(1);
	}
	return (1);
}

int	executebuiltin(t_commands *commands, t_inout_var var,
					int *o_fd, t_env *envi)
{
	pid_t	p;
	char	**cmd;
	int		i_fd;

	i_fd = var.input;
	p = fork();
	if (p == 0)
	{
		close (o_fd[0]);
		apply_redirection(commands->redirection, i_fd, o_fd[1], envi);
		cmd = &commands->command[var.i];
		if (strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
			echo(cmd);
		else if (strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
			env(&envi, cmd);
		else if (strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
			pwd();
		else if (strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
			export(cmd, &envi);
		paranoia_closing();
		exit_fork(envi, commands);
		exit(0);
	}
	return (0);
}
