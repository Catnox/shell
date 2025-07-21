/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mknoll <mknoll@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 15:14:28 by mknoll            #+#    #+#             */
/*   Updated: 2025/06/27 15:16:23 by mknoll           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_env *env)
{
	int	count;
	
	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

void	ft_sort_str_array(char **arr)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

char **export_env_to_array(t_env *env)
{
	int size = env_size(env);
	char **env_array = malloc(sizeof(char *) * (size + 1));
	int i = 0;
	t_env *current = env;
	if (!env_array)
		return NULL;
	while (current)
	{
		if (current->exported && current->value)
		{
			env_array[i] = malloc(ft_strlen(current->key) + ft_strlen(current->value) + 2);
			if (!env_array[i])
				return NULL;
			ft_snprintf(env_array[i], ft_strlen(current->key) + ft_strlen(current->value) + 2,
				"%s=%s", current->key, current->value);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return env_array;
}

static void print_and_free_env_array(char **env_array)
{
	int i = 0;
	while (env_array[i])
	{
		printf("declare -x %s\n", env_array[i]);
		free(env_array[i]);
		i++;
	}
	free(env_array);
}

int ft_env_sorted(t_env *env)
{
	char **env_array = export_env_to_array(env);
	if (!env_array)
		return (1);
	ft_sort_str_array(env_array);
	print_and_free_env_array(env_array);
	return (0);
}
