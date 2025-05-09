/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 01:09:57 by aharder           #+#    #+#             */
/*   Updated: 2025/04/26 01:34:47 by aharder          ###   ########.fr       */
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

void	process_commands(t_commands *commands, t_env *env,
		t_inout_var var)
{
	t_commands	*t;
	int			s;
	int			p_fd[2];

	t = commands;
	add_size_to_env(commands, env);
	while (t != NULL)
	{
		check_env(t->command, env, ft_sizedstr(t->command));
		pipe(p_fd);
		if (t->next == NULL)
		{
			close(p_fd[1]);
			p_fd[1] = 1;
		}
		var.i = first_not_null(t);
		s = execute(t, var, p_fd, env) % 255;
		add_exit_status(s, &env);
		do_i_close(p_fd[1], var.input, t);
		var.input = p_fd[0];
		if (t->next == NULL)
			close(p_fd[0]);
		t = t->next;
	}
}

int	createpipes(t_commands *commands, t_io_red *redirection, t_env *env)
{
	t_inout_var	var;

	(void)redirection;
	var.input = 0;
	var.output = 1;
	if (var.input == -1)
	{
		printf("Error: no input redirection\n");
		return (0);
	}
	process_commands(commands, env, var);
	return (1);
}
