/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmeli <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:45:14 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/17 11:02:45 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
void	*my_realloc(void *ptr, size_t old_size, size_t new_size)
{
	unsigned char	*src;
	unsigned char	*dst;
	void			*new_ptr;
	size_t			copy_size;
	size_t			i;

	if (ptr == NULL)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	printf("old & new_size in my_realloc: %ld %ld\n", old_size, new_size);
	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);
	if (old_size < new_size)
		copy_size = old_size;
	else
		copy_size = new_size;
	src = (unsigned char *)ptr;
	dst = (unsigned char *)new_ptr;
	i = 0;
	while (i < copy_size)
	{
		dst[i] = src[i];
		i++;
	}
	return (new_ptr);
}
*/

int	insert_string(char ***array, int *size, const char *str, int position)
{
	char	*new_str;
	int		i;

	if (array == NULL || size == NULL || str == NULL || position < 0
		|| position > *size)
		return (-1);
	new_str = strdup(str);
	if (new_str == NULL)
		return (-1);
	i = *size;
	while (i > position)
	{
		(*array)[i] = (*array)[i - 1];
		i--;
	}
	(*array)[position] = new_str;
	(*size)++;
	return (0);
}

void	ft_free_array_size(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	create_new_array(char ***array, char *arg, int index)
{
	char	**new_intermediate_array;
	int		i;
	int		size;
	int		size_new_array;

	size = count_strings_pointer(array);
	new_intermediate_array = ft_split(arg, ' ');
	size_new_array = size_of_array(new_intermediate_array);
	i = 0;
	while (i < size_new_array)
	{
		insert_string(array, &size, new_intermediate_array[i], index);
		i++;
		index++;
	}
	ft_free_array_size(new_intermediate_array, size_new_array);
	desert_string(array, &size, arg, index);
	return (size_new_array);
}

int	desert_string(char ***array, int *size, const char *str, int position)
{
	char	**current_array;
	int		i;

	(void)str;
	if (array == NULL || *array == NULL || size == NULL || *size <= 0
		|| position < 0 || position > *size)
		return (-1);
	current_array = *array;
	free(current_array[position]);
	i = position;
	while (i < *size - 1)
	{
		current_array[i] = current_array[i + 1];
		i++;
	}
	if (position == *size - 1)
		current_array[position] = NULL;
	current_array[i] = NULL;
	(*size)--;
	return (0);
}

void	echo_check(char ***args)
{
	int	size;
	int	i;
	int	jump;

	size = count_strings_pointer(args);
	i = 0;
	while (i < size)
	{
		if (there_is_space((*args)[i]) && scan_for_minus_n((*args)[i]))
		{
			if (!there_is_only_space((*args)[i]))
			{
				jump = create_new_array(args, (*args)[i], i);
				i = i + jump - 1;
				size = size + jump - 1;
			}
		}
		i++;
	}
}
