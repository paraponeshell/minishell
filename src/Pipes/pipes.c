/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 01:09:57 by aharder           #+#    #+#             */
/*   Updated: 2025/04/18 22:53:46 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_sizedstr(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
		i++;
	return (i);
}

void	process_commands(t_commands *commands, t_env *env, int b_fd[2], t_inout_var var)
{
	t_commands	*t;
	int			s;
	int			p_fd[2];

	t = commands;
	init_pipes(p_fd, b_fd);
	while (t != NULL)
	{
		check_env(t->command, env, ft_sizedstr(t->command));
		pipe(p_fd);
		if (t->next == NULL)
			p_fd[1] = 1;
		s = execute(t, var, p_fd, env) % 255;
		add_exit_status(s, &env);
		if (t->next != NULL)
		{
			if (t->next->pipe_type == 3 && s != 0)
				break ;
			if (t->next->pipe_type == 1 && s == 0)
				break ;
			close(p_fd[1]);
		}
		var.input = p_fd[0];
		t = t->next;
	}
	//print_commands(commands);
	//dup2(p_fd[0], b_fd[0]);
	//close_pipes(p_fd);
	//printf("ici");
}

int	createpipes(t_commands *commands, t_io_red *redirection, t_env *env)
{
	int	b_fd[2];
	t_inout_var	var;

	(void)redirection;
	var.input = 0;
	var.output = 1;
	b_fd[0] = 0;
	b_fd[1] = 0;
	//add_red_to_env(&redirection, &env);
	//add_cmd_to_env(&commands, &env);
	if (var.input == -1)
	{
		printf("Error: no input redirection\n");
		return (0);
	}
	//print_commands(commands);
	if (commands->command[0] != NULL && commands->command[0][0] != '\0')
		process_commands(commands, env, b_fd, var);
	//if (b_fd[1])
	//	close(b_fd[1]);
	//write_output(b_fd[0], redirection);
	return (1);
}

int	is_exec_command(char **str)
{
	char	*commands[3];
	int		i;

	i = 0;
	commands[0] = "echo";
	commands[1] = "env";
	commands[2] = "pwd";
	while (i < 3)
	{
		if (ft_strcmp(str[0], commands[i]) == 0)
			return (i);
		else if (ft_strcmp(str[0], "export") == 0 && str[1] == NULL)
			return (i);
		i++;
	}
	return (-1);
}

int	is_other_command(char *str)
{
	char	*commands[4];
	int		i;

	i = 0;
	commands[0] = "export";
	commands[1] = "unset";
	commands[2] = "cd";
	commands[3] = "exit";
	while (i < 4)
	{
		if (ft_strcmp(str, commands[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

void	init_pipes(int p_fd[2], int b_fd[2])
{
	pipe(b_fd);
	if (p_fd != NULL)
		pipe(p_fd);
}
