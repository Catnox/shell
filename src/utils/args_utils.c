/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(char **args)
{
	int	count;

	if (!args)
		return (0);
	
	count = 0;
	while (args[count])
		count++;
	
	return (count);
}

char	**dup_args(char **args)
{
	char	**new_args;
	int		count;
	int		i;

	if (!args)
		return (NULL);
	
	count = count_args(args);
	new_args = malloc(sizeof(char *) * (count + 1));
	if (!new_args)
		return (NULL);
	
	i = 0;
	while (i < count)
	{
		new_args[i] = ft_strdup(args[i]);
		if (!new_args[i])
		{
			free_args(new_args);
			return (NULL);
		}
		i++;
	}
	new_args[count] = NULL;
	
	return (new_args);
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
