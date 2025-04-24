/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:20:52 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 16:35:26 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_executable(t_commands *t, t_inout_var var)
{
	if (ft_strncmp(t->command[var.i], "./", 2) == 0)
		return (1);
	if (ft_strncmp(t->command[var.i], "../", 3) == 0)
		return (1);
	return (0);
}

int	commandbuiltin(t_commands *commands, t_inout_var var, int o_fd, t_env *env)
{
	int		exit_status;
	int		temp_o_fd;
	int		temp_i_fd;
	int		i_fd;

	i_fd = var.input;
	temp_o_fd = dup(STDOUT_FILENO);
	temp_i_fd = dup(STDIN_FILENO);
	apply_redirection(commands->redirection, i_fd, o_fd, env);
	exit_status = 0;
	if (strcmp(commands->command[var.i], "cd") == 0)
	{
		exit_status = cd(&commands->command[var.i], &env);
	}
	else if (strcmp(commands->command[var.i], "export") == 0)
		exit_status = export(&commands->command[var.i], &env);
	else if (strcmp(commands->command[var.i], "unset") == 0)
		exit_status = unset(&commands->command[var.i], &env);
	else if (strcmp(commands->command[var.i], "exit") == 0)
		exit_status = ft_exit(env, &commands->command[var.i]);
	dup2(temp_i_fd, STDIN_FILENO);
	dup2(temp_o_fd, STDOUT_FILENO);
	close(temp_i_fd);
	close(temp_o_fd);
	return (exit_status);
}

char	*get_path(char *cmd, t_env *env)
{
	int		i;
	char	*exec;
	char	**allpath;
	char	*path_part;

	i = -1;
	allpath = ft_split(ft_getenv(env, "PATH"), ':');
	if (allpath == NULL)
	{
		return (cmd);
	}
	while (allpath[++i])
	{
		path_part = ft_strjoin(allpath[i], "/");
		exec = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(exec, F_OK | X_OK) == 0)
		{
			return (exec);
		}
		free(exec);
	}
	free_split(allpath);
	return (cmd);
}
