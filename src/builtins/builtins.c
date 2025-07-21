/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 07:54:32 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/27 16:01:21 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_cd_path(char **argv, t_env *env)
{
	char	*path;

	if (!argv[1] || ft_strcmp(argv[1], "~") == 0)
	{
		path = get_variable_value_from_env(env, "HOME");
		if (!path)
		{
			perror("minishell: cd: HOME not set\n");
			return (NULL);
		}
	}
	else if (ft_strcmp(argv[1], "-") == 0)
	{
		path = get_variable_value_from_env(env, "OLDPWD");
		if (!path)
		{
			perror("minishell: cd: OLDPWD not set\n");
			return (NULL);
		}
	}
	else
		path = argv[1];
	return (path);
}

static void	update_pwd_vars(t_env **env)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env(env, "OLDPWD", get_variable_value_from_env(*env, "PWD"));
		set_env(env, "PWD", cwd);
	}
}

int	ft_cd(char **argv, t_env **env)
{
	char	*path;

	path = get_cd_path(argv, *env);
	if (!path)
		return (1);
	if (chdir(path) == -1)
		return (printf("minishell: cd: %s: %s\n", path, strerror(errno)), 1);
	update_pwd_vars(env);
	return (0);
}

int	ft_pwd(char **argv)
{
	char	cwd[1024];

	(void)argv;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	return (0);
}

int	handle_builtin(char **argv, t_env **my_env)
{
	if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, my_env));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(argv));
	else if (ft_strcmp(argv[0], "echo") == 0)
	{
		if (check_multiple_n(argv[1]))
			return (ft_echo_n(argv));
		else
			return (ft_echo(argv));
	}
	else if (ft_strcmp(argv[0], "export") == 0)
		return (handle_export(argv, my_env));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (handle_unset(argv, my_env));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env_custom(*my_env));
	else if (ft_strcmp(argv[0], "exit") == 0)
		return (ft_exit_simple(argv));
	else
	{
		print_error(argv[0], "command not found");
		return (127);
	}
}
