/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:55:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 01:34:16 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	execute(t_commands *t, t_inout_var var, int p_fd[2], t_env *env)
{
	int	status;
	int	i;

	status = 0;
	i = first_not_null(t);
	if (t->command[i] == NULL || t->command[i][0] == '\0')
		return (print_file_error(t->command[i]));
	if (t->command[i][0] == '/' && access(t->command[i], F_OK | X_OK) == 0)
		status = executefullfile(t, env, var.input, p_fd[1]);
	else if (ft_strncmp(t->command[i], "./", 2) == 0
		|| ft_strncmp(t->command[i], "../", 3) == 0)
	{
		if (access(&t->command[i][1], F_OK | X_OK))
			status = executefile(t, var.input, p_fd[1], env);
		else
			status = print_file_error(t->command[i]);
	}
	else if (is_exec_command(t->command[i]) != -1 || (ft_strcmp(t->command[i],
				"export") == 0 && t->command[i + 1] == NULL))
		status = executebuiltin(t, var.input, p_fd[1], env);
	else if (is_other_command(t->command[i]) != -1)
	{
		status = commandbuiltin(t, var.input, p_fd[1], env)
			* (is_only(env) == 1);
	}
	else
		status = executecommand(t, var.input, p_fd[1], env);
	return (status);
}

int	executefile(t_commands *command, int i_fd, int o_fd, t_env *env)
{
	extern char	**environ;
	char		*full_cmd;
	pid_t		p;
	int			exit_status;

	(void)env;
	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		apply_redirection(command->redirection, i_fd, o_fd, env);
		// dup2(i_fd, STDIN_FILENO);
		// dup2(o_fd, STDOUT_FILENO);
		if (ft_strncmp(command->command[0], "./", 2) == 0
			|| ft_strncmp(command->command[0], "../", 3) == 0)
			full_cmd = ft_relative_path(command->command[0]);
		else
			full_cmd = ft_strdup(command->command[0]);
		signal(SIGQUIT, handle_signal);
		execve(full_cmd, command->command, environ);
		free(full_cmd);
		exit(1);
	}
	return (exit_status);
}

int	executefullfile(t_commands *commands, t_env *env, int i_fd, int o_fd)
{
	int			exit_status;
	pid_t		p;
	extern char	**environ;

	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		if (!commands->command[0])
			exit(1);
		apply_redirection(commands->redirection, i_fd, o_fd, env);
		// dup2(i_fd, STDIN_FILENO);
		// dup2(o_fd, STDOUT_FILENO);
		signal(SIGQUIT, handle_signal);
		execve(commands->command[0], commands->command, environ);
		exit(1);
	}
	return (exit_status);
}

int	executecommand(t_commands *commands, int i_fd, int o_fd, t_env *env)
{
	int			exit_status;
	pid_t		p;
	extern char	**environ;
	char		*full_cmd;

	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		if (commands->command[0][0] == '\0')
			exit(1);
		apply_redirection(commands->redirection, i_fd, o_fd, env); 
		// dup2(i_fd, STDIN_FILENO);
		// dup2(o_fd, STDOUT_FILENO);
		full_cmd = get_path(commands->command[0], env);
		if (full_cmd == NULL)
			exit(1);
		signal(SIGQUIT, handle_signal);
		execve(full_cmd, commands->command, environ);
		perror("fail command");
		free(full_cmd);
		exit(1);
	}
	return (exit_status);
}

int	executebuiltin(t_commands *commands, int i_fd, int o_fd, t_env *envi)
{
	pid_t	p;
	char	**cmd;

	p = fork();
	if (p == 0)
	{
		// print_commands(commands);
		apply_redirection(commands->redirection, i_fd, o_fd, envi);
			// dup2(i_fd, STDIN_FILENO);
		// dup2(o_fd, STDOUT_FILENO);
		cmd = commands->command;
		if (strncmp(cmd[0], "echo", ft_strlen(cmd[0])) == 0)
			echo(cmd);
		else if (strncmp(cmd[0], "env", ft_strlen(cmd[0])) == 0)
			env(&envi, cmd);
		else if (strncmp(cmd[0], "pwd", ft_strlen(cmd[0])) == 0)
			pwd();
		else if (strncmp(cmd[0], "export", ft_strlen(cmd[0])) == 0)
			export(cmd, &envi);
		exit(0);
	}
	return (0);
}
