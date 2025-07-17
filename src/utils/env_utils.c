/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Stub implementations
int	set_env_value(t_env **env_list, char *key, char *value)
{
	(void)env_list;
	(void)key;
	(void)value;
	return (SUCCESS);
}

int	unset_env_value(t_env **env_list, char *key)
{
	(void)env_list;
	(void)key;
	return (SUCCESS);
}

char	**env_to_array(t_env *env_list)
{
	(void)env_list;
	return (NULL);
}
