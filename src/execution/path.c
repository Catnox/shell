/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	**all_paths;
	char	*full_path;
	char	*temp;
	int		i;

	// Si la commande contient un '/', c'est un chemin absolu ou relatif
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}

	// Utiliser get_env_value pour récupérer PATH
	path_env = get_env_value(env, "PATH");
	if (!path_env)
		return (NULL);

	all_paths = ft_split(path_env, ':');
	if (!all_paths)
		return (NULL);

	i = 0;
	while (all_paths[i])
	{
		temp = ft_strjoin(all_paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);

		if (access(full_path, F_OK | X_OK) == 0)
		{
			ft_free_tab(all_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}

	ft_free_tab(all_paths);
	return (NULL);
}
