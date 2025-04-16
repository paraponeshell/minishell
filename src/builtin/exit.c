/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:46:57 by aharder           #+#    #+#             */
/*   Updated: 2025/04/14 17:42:12 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_if_long(char *str)
{
	unsigned long long	result;
	int					sign;
	int					i;

	if (ft_strcmp(str, "-9223372036854775808") == 0)
		return (1);
	result = 0;
	sign = 1;
	i = 0;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-')
		sign = sign * -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + str[i] - '0';
		if (result > (unsigned long long)LLONG_MAX)
			return (0);
		if ((result > -(unsigned long long)LLONG_MIN) && (sign < 0))
			return (0);
		i++;
	}
	return (1);
}

long long	ft_atoi_long(char *str)
{
	unsigned long long	result;
	int					sign;
	int					i;

	if (strcmp(str, "-923372036854775808") == 0)
		return (0);
	result = 0;
	sign = 1;
	i = 0;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-')
		sign = sign * -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && ft_isdigit(str[i]))
	{
		result = result * 10 + str[i] - '0';
		if (result > (unsigned long long)LLONG_MAX)
			break ;
		if ((result > -(unsigned long long)LLONG_MIN) && (sign < 0))
			break ;
		i++;
	}
	return (result * sign);
}

int	check_arg_is_numeric(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (0);
	i = 0;
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(t_env *env, char **arg)
{
	void		*red;
	void		*commands;
	long long	exit_code;

	if (arg[1] != NULL && check_arg_is_numeric(arg[1]) && arg[2] != NULL)
		return (ft_putstr_fd("exit: too many arguments\n", 2));
	exit_code = 0;
	if (arg[1] && check_arg_is_numeric(arg[1]) == 1
		&& check_if_long(arg[1]) == 1)
		exit_code = ft_atoi_long(arg[1]) % 256 + (ft_atoi_long(arg[1]) < 0)
			* 256;
	if (arg[1] && (check_arg_is_numeric(arg[1]) == 0
			|| check_if_long(arg[1]) == 0))
	{
		ft_putstr_fd("error: exit: numeric argument required\n", 2);
		exit_code = 2;
	}
	red = str_to_ptr(ft_getallenv(env, "&"));
	commands = str_to_ptr(ft_getallenv(env, "+"));
	free_red(red);
	free_cmd(commands);
	free_env(env);
	ft_putstr_fd("exit\n", 2);
	exit(exit_code);
}
