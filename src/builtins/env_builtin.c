/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moritzknoll <moritzknoll@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:29:10 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/28 09:21:30 by moritzknoll      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **env)
{
	if (!env || !*env)
	{
		printf("minishell: env: no environment\n");
		return (1);
	}
	while (*env)
	{
		printf("%s\n", *env);
		env++;
	}
	return (0);
}

int	ft_env_custom(t_env *env)
{
	while (env)
	{
		if (env->value && env->exported)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

int	ft_unset(t_env **my_env, const char *key)
{
	t_env	*current;
	t_env	*previous;

	if (!key || !*key)
	{
		printf("minishell: unset: invalid identifier\n");
		return (1);
	}
	current = *my_env;
	previous = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				*my_env = current->next;
			else
				previous->next = current->next;
			return (free(current->value), free(current->key), free(current), 0);
		}
		previous = current;
		current = current ->next;
	}
	return (0);
}
