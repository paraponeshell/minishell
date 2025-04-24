/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:23:14 by aharder           #+#    #+#             */
/*   Updated: 2025/04/24 15:30:54 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	echo_space(char *arg)
{
	char	**array;
	char	*temp;

	temp = ft_strjoin("echo ", arg);
	free(arg);
	array = ft_split(temp, ' ');
	echo(array);
	free(array);
}

int	there_is_space(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

void	print_echo(char **args, int i)
{
	while (args[i])
	{
		if (ft_strcmp(args[i], "g%DH1211DVJQbaz") == 0)
			i++;
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
}

int	jsh_echo_intermed(char **args, int index)
{
	int	i;
	int	j;

	i = index;
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		j = 1;
		while (args[i][j] && args[i][j] == 'n')
		{
			j++;
		}
		if (args[i][j] == '\0')
			i++;
		else
			break ;
	}
	return (i);
}

int	echo(char **args)
{
	int	i;
	int	newline;
	int	j;

	i = 1;
	newline = 0;
	echo_check(&args);
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
	{
		j = 1;
		while (args[1][j] && args[1][j] == 'n')
		{
			j++;
		}
		if (args[1][j] == '\0')
		{
			newline++;
			i++;
		}
	}
	i = jsh_echo_intermed(args, i);
	print_echo(args, i);
	if (newline == 0)
		printf("\n");
	return (0);
}
