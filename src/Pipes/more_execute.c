/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_execute.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:20:52 by aharder           #+#    #+#             */
/*   Updated: 2025/04/23 12:40:47 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	commandbuiltin(t_commands *commands, int i_fd, int o_fd, t_env *env)
{
	int		exit_status;
	int		temp_o_fd;
	int		temp_i_fd;
	char	**arg;

	arg = commands->command;
	temp_o_fd = dup(STDOUT_FILENO);
	temp_i_fd = dup(STDIN_FILENO);
	apply_redirection(commands->redirection, i_fd, o_fd, env);
	exit_status = 0;
	if (strcmp(arg[0], "cd") == 0)
	{
		exit_status = cd(arg, &env);
	}
	else if (strcmp(arg[0], "export") == 0)
		exit_status = export(arg, &env);
	else if (strcmp(arg[0], "unset") == 0)
		exit_status = unset(arg, &env);
	else if (strcmp(arg[0], "exit") == 0)
		ft_exit(env, arg);
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
