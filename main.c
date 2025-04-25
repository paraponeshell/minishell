/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 02:27:04 by aharder           #+#    #+#             */
/*   Updated: 2025/04/25 15:36:34 by aharder          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

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

t_mini	init_mini(int argc, char **argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	mini.commands = NULL;
	mini.redirection = NULL;
	mini.env = init_env(envp);
	return (mini);
}

char	*compressed_main(void)
{
	char	*minishell;

	signal(SIGINT, handle_signal);
	block_signal(SIGQUIT);
	rl_on_new_line();
	minishell = readline("minishell : ");
	signal(SIGQUIT, handle_signal);
	unblock_signal(SIGQUIT);
	return (minishell);
}

int	main(int argc, char **argv, char **envp)
{
	char		*minishell;
	t_mini		mini;

	mini = init_mini(argc, argv, envp);
	while (1)
	{
		minishell = compressed_main();
		if (minishell == NULL)
		{
			rl_clear_history();
			free(minishell);
			break ;
		}
		if (is_empty_line(minishell) == 1)
		{
			if (minishell[0] != '\0')
				add_history(minishell);
			free(minishell);
			continue ;
		}
		add_history(minishell);
		signal(SIGINT, handle_signal_parser);
		parser(minishell, &mini);
		free(minishell);
	}
	free_env(mini.env);
}
