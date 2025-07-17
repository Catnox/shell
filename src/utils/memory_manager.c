/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Libère une liste de tokens de manière sécurisée
void	free_tokens_safe(t_token **tokens)
{
	t_token	*current;
	t_token	*next;
	
	if (!tokens || !*tokens)
		return;
	
	current = *tokens;
	while (current)
	{
		next = current->next;
		
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		
		free(current);
		current = next;
	}
	
	*tokens = NULL;
}

// Libère un AST de manière sécurisée
void	free_ast_safe(t_ast_node **root)
{
	if (!root || !*root)
		return;
	
	// Gère selon le type de nœud
	switch ((*root)->type)
	{
		case AST_COMMAND:
			if ((*root)->data.cmd)
			{
				if ((*root)->data.cmd->argv)
					free_string_array((*root)->data.cmd->argv);
				// Libère les redirections
				// Note: les redirections sont gérées par free_redir_list()
				free((*root)->data.cmd);
			}
			break;
			
		case AST_PIPE:
		case AST_AND:
		case AST_OR:
		case AST_SEQUENCE:
			// Libère les enfants récursivement
			if ((*root)->data.binary.left)
				free_ast_safe(&(*root)->data.binary.left);
			if ((*root)->data.binary.right)
				free_ast_safe(&(*root)->data.binary.right);
			break;
			
		case AST_SUBSHELL:
			if ((*root)->data.child)
				free_ast_safe(&(*root)->data.child);
			break;
	}
	
	free(*root);
	*root = NULL;
}

// Libère un tableau de chaînes de manière sécurisée
void	free_string_array(char **array)
{
	int	i;
	
	if (!array)
		return;
	
	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	
	free(array);
}

// Libère une liste d'environnement de manière sécurisée
void	free_env_list_safe(t_env **env_list)
{
	t_env	*current;
	t_env	*next;
	
	if (!env_list || !*env_list)
		return;
	
	current = *env_list;
	while (current)
	{
		next = current->next;
		
		if (current->key)
		{
			free(current->key);
			current->key = NULL;
		}
		
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		
		free(current);
		current = next;
	}
	
	*env_list = NULL;
}

// Gestion d'erreur avec nettoyage automatique
void	error_exit_safe(t_shell *shell, char *message, int exit_code)
{
	if (message)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(message, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	
	if (shell)
	{
		// Nettoie la structure shell
		if (shell->tokens)
			free_tokens_safe(&shell->tokens);
		
		if (shell->ast_root)
			free_ast_safe(&shell->ast_root);
		
		if (shell->env_list)
			free_env_list_safe(&shell->env_list);
		
		// Nettoie les fichiers temporaires
		cleanup_heredoc_files();
		
		// Ferme les descripteurs de fichiers
		if (shell->stdin_backup != -1)
			close(shell->stdin_backup);
		
		if (shell->stdout_backup != -1)
			close(shell->stdout_backup);
		
		// Libère l'historique readline
		clear_history();
	}
	
	exit(exit_code);
}

// Allocation sécurisée avec vérification
void	*safe_malloc(size_t size)
{
	void	*ptr;
	
	ptr = malloc(size);
	if (!ptr)
	{
		ft_putstr_fd("minishell: malloc failed\n", STDERR_FILENO);
		exit(FAILURE);
	}
	
	return (ptr);
}

// Réallocation sécurisée
void	*safe_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;
	
	new_ptr = realloc(ptr, new_size);
	if (!new_ptr && new_size > 0)
	{
		free(ptr);
		ft_putstr_fd("minishell: realloc failed\n", STDERR_FILENO);
		exit(FAILURE);
	}
	
	return (new_ptr);
}

// Duplication sécurisée de chaîne
char	*safe_strdup(const char *str)
{
	char	*dup;
	
	if (!str)
		return (NULL);
	
	dup = ft_strdup(str);
	if (!dup)
	{
		ft_putstr_fd("minishell: strdup failed\n", STDERR_FILENO);
		exit(FAILURE);
	}
	
	return (dup);
}

// Jointure sécurisée de chaînes
char	*safe_strjoin(const char *s1, const char *s2)
{
	char	*joined;
	
	if (!s1 || !s2)
		return (NULL);
	
	joined = ft_strjoin(s1, s2);
	if (!joined)
	{
		ft_putstr_fd("minishell: strjoin failed\n", STDERR_FILENO);
		exit(FAILURE);
	}
	
	return (joined);
}

// Nettoyage complet de la shell en cas d'interruption
void	cleanup_on_signal(t_shell *shell)
{
	if (!shell)
		return;
	
	// Nettoie les ressources sans exit
	if (shell->tokens)
		free_tokens_safe(&shell->tokens);
	
	if (shell->ast_root)
		free_ast_safe(&shell->ast_root);
	
	cleanup_heredoc_files();
	
	// Restaure les descripteurs de fichiers
	if (shell->stdin_backup != -1)
	{
		dup2(shell->stdin_backup, STDIN_FILENO);
		close(shell->stdin_backup);
		shell->stdin_backup = -1;
	}
	
	if (shell->stdout_backup != -1)
	{
		dup2(shell->stdout_backup, STDOUT_FILENO);
		close(shell->stdout_backup);
		shell->stdout_backup = -1;
	}
}
