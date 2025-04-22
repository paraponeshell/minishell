/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:55:16 by aharder           #+#    #+#             */
/*   Updated: 2025/04/22 15:38:50 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	apply_redirection(t_io_red *redirections, int i_fd, int o_fd, t_env *env)
{
    t_io_red	*current;
	int	o;
	int i;
	int	input_fd[2];
	int fd;
	
	o = 0;
	i = 0;
    current = redirections;
    while (current != NULL)
    {
		print_redirection(current);
        if (current->in_or_out == 5)
        {
			i++;
            fd = open(current->file, O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
		else if (current->in_or_out == 4) // Heredoc redirection
		{
			i++;
			pipe(input_fd);
			get_heredoc(input_fd, ft_strdup(current->file), env);
			close(input_fd[1]);
			dup2(input_fd[0], STDIN_FILENO);
		}
        else if (current->in_or_out == 7) // Output redirection
        {
			o++;
            fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (current->in_or_out == 6) // Append redirection
        {
			o++;
            fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror("open");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        current = current->next;
    }
	//printf("i_fd: %d, o_fd: %d\n", i_fd, o_fd);
	//printf("i: %d, o: %d\n", i, o);
	if (i == 0 && i_fd != 0)
	{
		dup2(i_fd, STDIN_FILENO);
		close(i_fd);
	}
	if (o == 0 && o_fd != 1)
	{
		dup2(o_fd, STDOUT_FILENO);
		close(o_fd);
	}
	return ;
}

int	is_only(t_env *env)
{
	int	size;
	char	*var;

	var = ft_getallenv(env, "#");
	size = ft_atoi(var);
	return (size);
}

int	execute(t_commands *t, t_inout_var var, int p_fd[2], t_env *env)
{
	int	status;

	print_commands(t);
	status = 0;
	if (t->command[0][0] == '/' && access(t->command[0], F_OK | X_OK) == 0)
		status = executefullfile(t, env, var.input, p_fd[1]);
	else if (ft_strncmp(t->command[0], "./", 2) == 0 || ft_strncmp(t->command[0], "../", 3) == 0)
	{
		if (access(&t->command[0][1], F_OK | X_OK))
			status = executefile(t, var.input, p_fd[1], env);
		else
			status = print_file_error(t->command[0]);
	}
	else if (is_exec_command(t->command) != -1)
		status = executebuiltin(t, var.input, p_fd[1], env);
	else if (is_other_command(t->command[0]) != -1)
	{
		if (is_only(env) == 1)
			status = commandbuiltin(t, var.input, p_fd[1], env);
	}
	else
		status = executecommand(t, var.input, p_fd[1], env);
	return (status);
}

int	executefile(t_commands *command, int i_fd, int o_fd, t_env *env)
{
	extern char	**environ;
	char		*full_cmd;
	pid_t			p;
	int				exit_status;

	(void)env;
	p = fork();
	exit_status = 1;
	if (p == 0)
	{
		apply_redirection(command->redirection, i_fd, o_fd, env);
		//dup2(i_fd, STDIN_FILENO);
		//dup2(o_fd, STDOUT_FILENO);
		if (ft_strncmp(command->command[0], "./", 2) == 0 || ft_strncmp(command->command[0], "../", 3) == 0)
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
		//dup2(i_fd, STDIN_FILENO);
		//dup2(o_fd, STDOUT_FILENO);
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
		apply_redirection(commands->redirection, i_fd, o_fd, env);		//dup2(i_fd, STDIN_FILENO);
		//dup2(o_fd, STDOUT_FILENO);
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
	pid_t		p;
	char		**cmd;
	
	p = fork();
	if (p == 0)
	{
		//print_commands(commands);
		apply_redirection(commands->redirection, i_fd, o_fd, envi);		//dup2(i_fd, STDIN_FILENO);
		//dup2(o_fd, STDOUT_FILENO);
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
