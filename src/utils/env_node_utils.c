/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_node_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	remove_env_node(t_env **env_list, t_env *current, t_env *prev)
{
	if (prev)
		prev->next = current->next;
	else
		*env_list = current->next;
	free(current->key);
	if (current->value)
		free(current->value);
	free(current);
}

t_env	*find_env_node(t_env *env_list, char *key, t_env **prev)
{
	t_env	*current;

	current = env_list;
	*prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current);
		*prev = current;
		current = current->next;
	}
	return (NULL);
}
