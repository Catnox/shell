/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_processes(t_shell *shell)
{
	int	i;
	int	status;

	if (!shell->pids)
		return ;

	// Wait for all child processes
	i = 0;
	while (i < shell->pid_count)
	{
		if (shell->pids[i] > 0)
			waitpid(shell->pids[i], &status, 0);
		i++;
	}

	free(shell->pids);
	shell->pids = NULL;
	shell->pid_count = 0;
}

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;

	// Clean up processes
	cleanup_processes(shell);

	// Clean up environment list
	if (shell->env_list)
	{
		free_env_list_safe(&shell->env_list);
		shell->env_list = NULL;
	}

	// Close backup file descriptors
	if (shell->stdin_backup != -1)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}

	if (shell->stdout_backup != -1)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}

	// Clear readline history
	clear_history();

	printf("Shell cleaned up successfully. Goodbye! 👋\n");
}
