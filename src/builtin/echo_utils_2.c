/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <jmeli@student.42luxembourg.lu>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:32:46 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/17 11:01:41 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_strings_pointer(char ***array_ptr)
{
	int		count;
	char	**array;

	count = 0;
	if (array_ptr == NULL)
		return (0);
	array = *array_ptr;
	if (array == NULL)
		return (0);
	while (array[count] != NULL)
	{
		count++;
	}
	return (count);
}

/*
void	ft_print_array_of_strings(char **array, int size)
{
	int	i;

	if (array == NULL)
		return ;
	i = 0;
	while (i < size)
	{
		ft_putstr_fd("array[", 1);
		ft_putnbr_fd(i, 1);
		ft_putstr_fd("]: ", 1);
		ft_putstr_fd(array[i], 1);
		ft_putchar_fd(' ', 1);
		ft_putnbr_fd(size, 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}
*/

int	size_of_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
		i++;
	return (i);
}

int	there_is_only_space(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] != 32)
			return (0);
		i++;
	}
	return (1);
}

int	scan_for_minus_n(char *arg)
{
	int	i;
	int	j;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '-')
		{
			j = i + 1;
			while (arg[j] && arg[j] == 'n')
			{
				j++;
			}
			if (arg[j] == 32)
				return (1);	
		}	
		i++;
	}
	return (0);
}
