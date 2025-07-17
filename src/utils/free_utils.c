/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

void	free_command_list(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		free_args(cmd->argv);
		free(cmd);
		cmd = next;
	}
}

void	cleanup_all(t_token *tokens, t_command *cmd_list,
			char *line, t_env **env)
{
	if (tokens)
		free_tokens(tokens);
	if (cmd_list)
		free_command_list(cmd_list);
	if (line)
		free(line);
	if (env && *env)
	{
		free_env(*env);
		*env = NULL;
	}
	cleanup_heredoc_files();
	rl_clear_history();
}
