/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 16:08:07 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 13:23:06 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char *ft_strrmquotes(char *str, char to_rm)
{
	char	*output;
	int		i;
	int		j;

	i = 0;
	j = 0;
	output = malloc((ft_strlen(str)) * sizeof(char));
	while (str[i] != '\0')
	{
		if (str[i] != to_rm)
			output[j++] = str[i];
		i++;
	}
	free(str);
	output[j] = '\0';
	return (output);
}

void	get_heredoc(int *p_fd, char *end, t_env *env)
{
	char	*heredoc;
	int		replace;

	replace = 1;
	if (end[0] == '\'')
			replace = 0;
	if (end[0] == '\'' || end[0] == '"')
			end = ft_strrmquotes(end, end[0]);
	while (1)
	{
		heredoc = readline("> ");
		if (!heredoc || ft_strcmp(heredoc, end) == 0)
		{
			write(p_fd[1], "\n", 1);
			free(heredoc);
			break ;
		}
		if (replace)
			check_env(&heredoc, env, 1);
		write(p_fd[1], heredoc, ft_strlen(heredoc));
		free(heredoc);
	}
	free(end);
}

int	find_i_red(t_io_red *redirection, t_env *env)
{
	t_io_red	*temp;
	int			input_fd[2];

	temp = redirection;
	input_fd[0] = 0;
	if (temp == NULL)
		return (0);
	while (temp != NULL)
	{
		if (temp->in_or_out == 5)
		{
			if (input_fd[0])
				close(input_fd[0]);
			input_fd[0] = open(temp->file, O_RDONLY);
		}
		if (temp->in_or_out == 4)
		{
			if (input_fd[0])
				close(input_fd[0]);
			pipe(input_fd);
			get_heredoc(input_fd, ft_strdup(temp->file), env);
			close(input_fd[1]);
		}
		temp = temp->next;
	}
	return (input_fd[0]);
}
