/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aharder <aharder@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 18:29:13 by aharder           #+#    #+#             */
/*   Updated: 2025/04/23 12:15:39 by jmeli            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*get_root_directory(t_env **env)
{
	t_env	*ptr;

	ptr = *env;
	while (ptr)
	{
		if (ft_strncmp(ptr->value, "HOME", 4) == 0)
		{
			if (ptr->result && ft_strcmp(ptr->result, "") != 0)
				return (ft_strdup(ptr->result));
		}
		ptr = ptr->next;
	}
	return (NULL);
}

int	update_pwd(t_env **env)
{
	char	**args;
	char	*pwd;
	char	*copy;
	char	buf[1096];

	pwd = getcwd(buf, 1096);
	if (pwd == NULL)
		return (1);
	args = malloc(3 * sizeof(char *));
	if (args == NULL)
		return (1);
	args[0] = ft_strdup("export");
	copy = ft_strdup(pwd);
	args[1] = ft_strjoin("PWD=", copy);
	args[2] = NULL;
	export(args, env);
	free(args[0]);
	free(copy);
	free(args[1]);
	free(args);
	return (0);
}

void	update_old_pwd(t_env **env, char *oldwd)
{
	char	**args;
	char	*copy;

	if (oldwd == NULL || oldwd[0] == '\0')
		return ;
	args = malloc(3 * sizeof(char *));
	args[0] = ft_strdup("export");
	copy = ft_strdup(oldwd);
	args[1] = ft_strjoin("OLDPWD=", copy);
	args[2] = NULL;
	export(args, env);
	free(args[0]);
	free(copy);
	free(args[1]);
	free(args);
}

int	cd_root(char *cwd, t_env **env)
{
	char	*root;
	int		exit_status;

	exit_status = 0;
	root = get_root_directory(env);
	if (root == NULL || chdir(root) != 0)
	{
		if (home_is_set_but_no_value(env))
			return (1);
		ft_putendl_fd("cd: root error", 2);
		return (1);
	}
	else
	{
		if (cwd != NULL)
			update_old_pwd(env, cwd);
		else
			update_old_with_current(env);
		exit_status = update_pwd(env);
		free(root);
		return (exit_status);
	}
	return (1);
}

int	cd(char **args, t_env **env)
{
	char	*cwd;
	char	buf[1096];

	cwd = getcwd(buf, 1096);
	if ((!args[1] && args[1] == NULL) || (args[1] && ft_strcmp(args[1],
				"~") == 0) || ft_strcmp(args[1], "") == 0)
		return (cd_root(cwd, env));
	else if (args[2])
	{
		ft_putendl_fd("cd: too many arguments", 2);
		return (1);
	}
	else if (chdir(args[1]) != 0)
	{
		ft_putendl_fd("cd : error", 2);
		return (1);
	}
	update_old_pwd(env, cwd);
	update_pwd(env);
	return (0);
}
