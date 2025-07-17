/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_executor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Vérifie si le chemin est un répertoire
static int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
		return (1);
	return (0);
}

// Vérifie les permissions et l'existence
static int	check_permissions(char *cmd_path)
{
	if (access(cmd_path, F_OK) != 0)
	{
		print_error(cmd_path, "No such file or directory\n");
		g_exit_status = 127;
		return (0);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		print_error(cmd_path, "Permission denied\n");
		g_exit_status = 126;
		return (0);
	}
	if (is_directory(cmd_path))
	{
		print_error(cmd_path, "Is a directory\n");
		g_exit_status = 126;
		return (0);
	}
	return (1);
}

// Processus enfant pour l'exécution externe
static void	child_process(char *path, char **args, t_env *env)
{
	char	**env_array;

	reset_signals_to_default();  // Utiliser la fonction de votre ami
	env_array = env_to_array(env);
	if (execve(path, args, env_array) == -1)
	{
		perror("execve");
		free_args(env_array);
		free(path);
		exit(127);
	}
}

// Processus parent - attendre l'enfant
static void	parent_process(pid_t pid, char *path)
{
	int	status;

	ignore_sigint();  // Exactement comme votre ami
	waitpid(pid, &status, 0);
	free(path);
	
	// Traitement du status comme votre ami
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_status = 128 + WTERMSIG(status);
	
	handle_status_and_print(status);  // Affichage des signaux
}

// Lance une commande externe
static void	launch_extern_command(char **args, t_env *env)
{
	pid_t	pid;
	char	*path;

	if (!args || !args[0])
	{
		g_exit_status = 127;
		return ;
	}

	// Si le chemin contient '/', utiliser tel quel
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = find_command_path(args[0], env);

	if (!path)
	{
		print_error(args[0], "command not found\n");
		g_exit_status = 127;
		return ;
	}

	if (!check_permissions(path))
	{
		free(path);
		return ;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_status = 1;
		free(path);
		return ;
	}

	if (pid == 0)
		child_process(path, args, env);
	else
		parent_process(pid, path);
}

// Exécute une commande simple (inspiré du code de votre ami)
int	execute_simple_cmd(t_simple_cmd *cmd, t_env **env)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);

	// Traiter les heredocs d'abord
	if (process_heredocs_simple(cmd, *env) < 0)
		return (1);

	// Vérifier si c'est un builtin
	if (is_builtin(cmd->argv[0]))
	{
		// Pour les builtins, on pourrait ajouter la gestion des redirections ici
		return (handle_builtin(cmd->argv, env));
	}

	// Commande externe
	launch_extern_command(cmd->argv, *env);
	return (g_exit_status);
}

// Executor AST simple et robuste
int	execute_ast_simple(t_ast_node *node, t_env **env)
{
	if (!node)
		return (0);

	switch (node->type)
	{
		case AST_COMMAND:
			if (node->data.cmd)
				return (execute_simple_cmd(node->data.cmd, env));
			break;
			
		case AST_PIPE:
			// Pour l'instant, exécuter simplement la commande de gauche
			// TODO: Implémenter les pipes plus tard
			if (node->data.binary.left)
				return (execute_ast_simple(node->data.binary.left, env));
			break;
			
		case AST_AND:
			// cmd1 && cmd2
			if (node->data.binary.left)
			{
				int left_status = execute_ast_simple(node->data.binary.left, env);
				if (left_status == 0 && node->data.binary.right)
					return (execute_ast_simple(node->data.binary.right, env));
				return (left_status);
			}
			break;
			
		case AST_OR:
			// cmd1 || cmd2
			if (node->data.binary.left)
			{
				int left_status = execute_ast_simple(node->data.binary.left, env);
				if (left_status != 0 && node->data.binary.right)
					return (execute_ast_simple(node->data.binary.right, env));
				return (left_status);
			}
			break;
			
		case AST_SEQUENCE:
			// cmd1 ; cmd2
			if (node->data.binary.left)
				execute_ast_simple(node->data.binary.left, env);
			if (node->data.binary.right)
				return (execute_ast_simple(node->data.binary.right, env));
			break;
			
		case AST_SUBSHELL:
			// (cmd)
			if (node->data.child)
				return (execute_ast_simple(node->data.child, env));
			break;
	}
	
	return (0);
}
