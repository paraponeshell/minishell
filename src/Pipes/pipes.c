/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 01:09:57 by aharder           #+#    #+#             */
/*   Updated: 2025/04/23 10:49:34 by jmeli            ###   ########.fr       */
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

void	add_size_to_env(t_commands *commands, t_env *env)
{
	int		i;
	char	*str;
	int		index;
	char	*buffer;

	i = 0;
	while (commands != NULL)
	{
		i++;
		commands = commands->next;
	}
	buffer = ft_itoa(i);
	str = ft_strjoin("#=", buffer);
	free(buffer);
	index = -1;
	index = index_existing_var(str, &env);
	if (index >= 0)
		update(str, index, &env);
	else
	{
		ft_env_push_back(&env, str);
	}
	free(str);
}

void	process_commands(t_commands *commands, t_env *env, int b_fd[2],
		t_inout_var var)
{
	t_commands	*t;
	int			s;
	int			p_fd[2];

	t = commands;
	init_pipes(p_fd, b_fd);
	add_size_to_env(commands, env);
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
			if ((t->next->pipe_type == 3 && s != 0) || (t->next->pipe_type == 1
					&& s == 0))
				break ;
			close(p_fd[1]);
		}
		var.input = p_fd[0];
		t = t->next;
	}
}

int	createpipes(t_commands *commands, t_io_red *redirection, t_env *env)
{
	int			b_fd[2];
	t_inout_var	var;

	(void)redirection;
	var.input = 0;
	var.output = 1;
	b_fd[0] = 0;
	b_fd[1] = 0;
	if (var.input == -1)
	{
		printf("Error: no input redirection\n");
		return (0);
	}
	if (commands->command[0] != NULL && commands->command[0][0] != '\0')
		process_commands(commands, env, b_fd, var);
	return (1);
}
