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

static size_t	get_string_length(const char *s)
{
	size_t	len;

	len = 0;
	while (s && s[len])
		len++;
	return (len);
}

static void	copy_string_to_result(char *result, const char *s, size_t offset)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		result[offset + i] = s[i];
		i++;
	}
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;
	size_t	len1;
	size_t	len2;

	len1 = get_string_length(s1);
	len2 = get_string_length(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
		return (NULL);
	copy_string_to_result(result, s1, 0);
	copy_string_to_result(result, s2, len1);
	result[len1 + len2] = '\0';
	return (free(s1), result);
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
