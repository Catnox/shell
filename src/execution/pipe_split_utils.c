/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_split_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**create_command_from_args(char **args, int start, int end)
{
	char	**command;
	int		arg_index;

	command = malloc(sizeof(char *) * (end - start + 1));
	if (!command)
		return (NULL);
	arg_index = 0;
	while (start < end)
	{
		command[arg_index] = ft_strdup(args[start]);
		arg_index++;
		start++;
	}
	command[arg_index] = NULL;
	return (command);
}

void	process_pipe_segment(char ***commands, char **args, int *cmd_index, 
		int *start, int i)
{
	commands[*cmd_index] = create_command_from_args(args, *start, i);
	(*cmd_index)++;
	*start = i + 1;
}
