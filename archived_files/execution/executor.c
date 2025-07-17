/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_ast(t_ast_node *node, t_env **env)
{
	if (!node)
		return (SUCCESS);
	
	if (node->type == AST_COMMAND)
		return (execute_single_command(node, env));
	
	// Add other node types later
	return (SUCCESS);
}

int	execute_single_command(t_ast_node *node, t_env **env)
{
	if (!node || node->type != AST_COMMAND || !node->data.cmd || 
		!node->data.cmd->argv || !node->data.cmd->argv[0])
		return (SUCCESS);
	
	// Traiter les heredocs avant l'exécution
	if (node->data.cmd->redirs && process_heredocs_simple(node->data.cmd, *env) < 0)
		return (FAILURE);
	
	// Check if it's a builtin command
	if (is_builtin(node->data.cmd->argv[0]))
	{
		int stdin_backup, stdout_backup;
		int result;
		
		// Sauvegarder les descripteurs standard
		save_std_fds(&stdin_backup, &stdout_backup);
		
		// Appliquer les redirections
		if (node->data.cmd->redirs && apply_redirections(node->data.cmd->redirs) < 0)
		{
			restore_std_fds(stdin_backup, stdout_backup);
			return (FAILURE);
		}
		
		// Exécuter le builtin
		result = execute_builtin_cmd(node->data.cmd->argv, env);
		
		// Restaurer les descripteurs standard
		restore_std_fds(stdin_backup, stdout_backup);
		
		return (result);
	}
	
	// Execute external command with redirections
	return (execute_simple_command(node->data.cmd, env));
}

int	execute_builtin_cmd(char **args, t_env **env)
{
	if (!args || !args[0])
		return (FAILURE);
	
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, env));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env_custom(*env));
	if (ft_strcmp(args[0], "export") == 0)
		return (handle_export(args, env));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(env, args[1]));
	if (ft_strcmp(args[0], "help") == 0)
		return (builtin_help());
	
	// Other builtins - stubs for now
	printf("%s: function not implemented yet\n", args[0]);
	return (SUCCESS);
}

int	builtin_env_new(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->exported)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (SUCCESS);
}

int	builtin_exit_new(char **args, t_env **env)
{
	int	exit_code;

	printf("exit\n");
	
	exit_code = g_exit_status;
	if (args[1])
	{
		if (ft_isnumeric(args[1]))
			exit_code = ft_atoi(args[1]);
		else
		{
			print_error("exit", "numeric argument required");
			exit_code = 2;
		}
	}
	
	cleanup_all(NULL, NULL, NULL, env);
	exit(exit_code);
}
