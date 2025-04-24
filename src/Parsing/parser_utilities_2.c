/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utilities_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:46 by jmeli             #+#    #+#             */
/*   Updated: 2025/04/24 16:29:26 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	*get_operators(char *s)
{
	int	i[4];
	int	*output;

	i[0] = 0;
	i[1] = 1;
	i[2] = 0;
	i[3] = 0;
	output = malloc((splitlen(s, ' ') + 1000) * sizeof(int));
	output[0] = 2;
	if (!output)
		return (NULL);
	output = get_op_loop(s, i, output);
	output[i[1]] = '\0';
	return (output);
}

void	print_commands(t_commands *commands)
{
	t_commands	*current;
	int			i;
	t_io_red	*red;

	current = commands;
	i = 1;
	while (current)
	{
		printf("Command: %s\n", current->command[0]);
		while (current->command[i] != NULL)
		{
			printf("  Arg[%d]: %s\n", i, current->command[i]);
			i++;
		}
		red = current->redirection;
		while (red)
		{
			printf("  Redirection: type=%d, file=%s\n", red->in_or_out,
				red->file);
			red = red->next;
		}
		current = current->next;
	}
}

void	print_redirection(t_io_red *redirection)
{
	t_io_red	*current;

	current = redirection;
	while (current != NULL)
	{
		printf("IO Type: %d\n", current->in_or_out);
		printf("File: %s\n", current->file);
		current = current->next;
	}
}

void	print_split(char **split)
{
	int	i;

	i = 0;
	while (split[i] != NULL)
	{
		printf("Split[%d]: %s\n", i, split[i]);
		i++;
	}
}

int	*get_op_loop(char *s, int *i, int *output)
{
	while (s[i[0]] != '\0')
	{
		if (s[i[0]] == '"' && i[3] == 0)
		{
			i[2] = !i[2];
			i[0]++;
		}
		else if (s[i[0]] == '\'' && i[2] == 0)
		{
			i[3] = !i[3];
			i[0]++;
		}
		else if (cmp(s[i[0]]) && i[2] == 0 && i[3] == 0)
		{
			if (!handle_operator(s, &i[0], &i[1], output))
			{
				free(output);
				return (NULL);
			}
		}
		else
			i[0]++;
	}
	return (output);
}
