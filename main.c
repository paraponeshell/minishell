/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 02:27:04 by aharder           #+#    #+#             */
/*   Updated: 2025/04/16 16:19:50 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

void	block_signal(int signal)
{
    struct sigaction sa;

    sa.sa_handler = SIG_IGN; // Ignore le signal
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, NULL);
}

void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handle_signal_parser(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
}

void	unblock_signal(int signal)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL; // Rétablit le comportement par défaut
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, NULL);
}
/*
char	*crop_path(char **path)
{
	int		i;
	char	*buffer;
	char	*buffer2;

	i = 0;
	while (path[i] != NULL)
		i++;
	buffer = ft_strjoin(path[i - 2], "/");
	buffer2 = ft_strjoin(buffer, path[i - 1]);
	free(buffer);
	return (buffer2);
}

char	*get_prompt(void)
{
	char	path[1024];
	char	*buffer;
	char	**buffer2;
	char	*prompt = NULL;
	char	*prefix = "minishell:~/";
	char	*suffix = "$";

	if (getcwd(path, sizeof(path)) != NULL)
	{
		buffer2 = ft_split(path, '/');
		prompt = crop_path(buffer2);
		free_split(buffer2);
		buffer = ft_strjoin(prefix, prompt);
		free(prompt);
		prompt = ft_strjoin(buffer, suffix);
		free(buffer);
	}
	return (prompt);
}*/

void	print_mini(void)
{
	printf("___  ________ _   _ _____ _____ _   _  _____ _      _     \n");
	printf("|  \\/  |_   _| \\ | |_   _/  ___| | | ||  ___| |    | |    \n");
	printf("| .  . | | | |  \\| | | | \\ `--.| |_| || |__ | |    | |    \n");
	printf("| |\\/| | | | | . ` | | |  `--. \\  _  ||  __|| |    | |    \n");
	printf("| |  | |_| |_| |\\  |_| |_/\\__/ / | | || |___| |____| |____\n");
	printf("\\_|  |_/\\___/\\_| \\_/\\___/");
	printf("\\____/\\_| |_/\\____/\\_____/\\_____/\n");
}

int	is_empty_line(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	char		*minishell;
	t_mini		mini;
	int			i;
	(void)argc;
	(void)argv;
	mini.commands = NULL;
	mini.redirection = NULL;
	mini.env = init_env(envp);
	i = 0;
	while (1)
	{
		signal(SIGINT, handle_signal);
		block_signal(SIGQUIT);
		rl_on_new_line();
		minishell = readline("minishell : ");
		signal(SIGQUIT, handle_signal);
		unblock_signal(SIGQUIT);
		if (minishell == NULL)
		{
			printf("Readline returned NULL\n");
			free(minishell);
			break ;
		}
		if (is_empty_line(minishell) == 1)
		{
			free(minishell);
			continue ;
		}
		add_history(minishell);
		signal(SIGINT, handle_signal_parser);
		if (minishell)
			parser(minishell, &mini);
		free(minishell);
		i++;
	}
	free_env(mini.env);
}
