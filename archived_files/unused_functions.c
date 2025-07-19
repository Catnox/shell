/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unused_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * Ce fichier contient les implémentations des fonctions déclarées dans 
 * minishell.h mais qui ne sont plus utilisées dans l'architecture actuelle.
 * 
 * Ces fonctions sont archivées au cas où elles seraient nécessaires 
 * pour des développements futurs ou des extensions.
 * 
 * Date d'archivage: 19 juillet 2025
 * Raison: Migration vers architecture simple, abandon de l'AST complexe
 */

#include "minishell.h"

/* ========================= FONCTIONS PRINCIPALES NON UTILISÉES ========================= */

// Remplacée par simple_minishell_loop
void	minishell_loop(t_env **my_env, char **envp)
{
	(void)my_env;
	(void)envp;
	// Implementation would go here - not used in simple architecture
}

// Architecture AST abandonnée
void	shell_loop(t_shell *shell)
{
	(void)shell;
	// Implementation would go here - AST architecture abandoned
}

// Remplacée par l'architecture simple
void	process_input(char *line, t_env **my_env, char **env)
{
	(void)line;
	(void)my_env;
	(void)env;
	// Implementation would go here - replaced by simple architecture
}

/* ========================= FONCTIONS DE PARSING NON UTILISÉES ========================= */

// Remplacé par parsing simple
t_command	*parse_commands(t_token *tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
void	merge_token(t_token **tokens)
{
	(void)tokens;
}

// Non utilisé
void	strip_quotes_inplace(t_token *tokens)
{
	(void)tokens;
}

// Non utilisé
t_command	*build_command(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
int	fill_command_argv_and_redirs(t_command *cmd, t_token **tok)
{
	(void)cmd;
	(void)tok;
	return (0);
}

/* ========================= UTILITAIRES DE CHEMIN NON UTILISÉS ========================= */

// Remplacé par find_command_path
char	*get_path(char *cmd, char *envp[])
{
	(void)cmd;
	(void)envp;
	return (NULL);
}

// Non utilisé
char	**split_path(char *envp[])
{
	(void)envp;
	return (NULL);
}

// Non utilisé
char	*get_env_path(char *env[])
{
	(void)env;
	return (NULL);
}

/* ========================= FONCTIONS D'EXÉCUTION AST NON UTILISÉES ========================= */

// Architecture AST abandonnée
int	execute_ast(t_ast_node *node, t_env **env)
{
	(void)node;
	(void)env;
	return (0);
}

// Remplacée par architecture simple
int	execute_single_command(t_ast_node *node, t_env **env)
{
	(void)node;
	(void)env;
	return (0);
}

// Remplacée par handle_builtin
int	execute_builtin_cmd(char **args, t_env **env)
{
	(void)args;
	(void)env;
	return (0);
}

// Non utilisée
void	setup_redirections(t_redirect *redirects, t_shell *shell)
{
	(void)redirects;
	(void)shell;
}

// Non utilisée
int	is_builtin_cmd(char *cmd)
{
	(void)cmd;
	return (0);
}

/* ========================= FONCTIONS AST NON UTILISÉES ========================= */

// Architecture AST abandonnée
t_ast_node	*create_ast_node(e_ast_type type)
{
	(void)type;
	return (NULL);
}

// Non utilisé
t_simple_cmd	*command_to_simple_cmd(t_command *cmd)
{
	(void)cmd;
	return (NULL);
}

// Non utilisé
t_ast_node	*parse_simple_command(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
t_ast_node	*parse_pipe(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
t_ast_node	*parse_and(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
t_ast_node	*parse_or(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

// Non utilisé
t_ast_node	*parse_sequence(t_token **tokens)
{
	(void)tokens;
	return (NULL);
}

/* ========================= FONCTIONS D'EXÉCUTION AST NON UTILISÉES ========================= */

// Remplacée par execute_pipe_chain
int	execute_pipe(t_ast_node *left, t_ast_node *right, t_env **env)
{
	(void)left;
	(void)right;
	(void)env;
	return (0);
}

// Non utilisé
int	execute_ast_new(t_ast_node *node, t_env **env)
{
	(void)node;
	(void)env;
	return (0);
}

// Non utilisé
void	free_simple_cmd(t_simple_cmd *cmd)
{
	(void)cmd;
}

// Non utilisé
void	free_ast_new(t_ast_node *node)
{
	(void)node;
}

// Non utilisé
int	process_heredocs(t_command *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
	return (0);
}

// Non utilisé
int	process_heredocs_simple(t_simple_cmd *cmd, t_env *env)
{
	(void)cmd;
	(void)env;
	return (0);
}

/* ========================= FONCTIONS DE REDIRECTION NON UTILISÉES ========================= */

// Remplacées par l'architecture simple
int	apply_redirection(t_redir *redir)
{
	(void)redir;
	return (0);
}

// Remplacées par l'architecture simple
int	apply_redirections(t_redir *redirs)
{
	(void)redirs;
	return (0);
}

// Non utilisées
void	save_std_fds(int *stdin_backup, int *stdout_backup)
{
	(void)stdin_backup;
	(void)stdout_backup;
}

// Non utilisées
void	restore_std_fds(int stdin_backup, int stdout_backup)
{
	(void)stdin_backup;
	(void)stdout_backup;
}

/* ========================= FONCTIONS ROBUSTES NON UTILISÉES ========================= */

// Non utilisé
void	free_tokens_safe(t_token **tokens)
{
	(void)tokens;
}

// Non utilisé
void	free_ast_safe(t_ast_node **root)
{
	(void)root;
}

// Non utilisé
void	free_string_array(char **array)
{
	(void)array;
}

// Non utilisé
void	free_env_list_safe(t_env **env_list)
{
	(void)env_list;
}

// Non utilisé
void	error_exit_safe(t_shell *shell, char *message, int exit_code)
{
	(void)shell;
	(void)message;
	(void)exit_code;
}

// Non utilisé
void	*safe_malloc(size_t size)
{
	(void)size;
	return (NULL);
}

// Non utilisé
void	*safe_realloc(void *ptr, size_t new_size)
{
	(void)ptr;
	(void)new_size;
	return (NULL);
}

// Non utilisé
char	*safe_strdup(const char *str)
{
	(void)str;
	return (NULL);
}

// Non utilisé
char	*safe_strjoin(const char *s1, const char *s2)
{
	(void)s1;
	(void)s2;
	return (NULL);
}

// Non utilisé
void	cleanup_on_signal(t_shell *shell)
{
	(void)shell;
}

/* ========================= FONCTIONS HEREDOC ROBUSTES NON UTILISÉES ========================= */

// Non utilisé
char	*handle_heredoc_robust(char *delimiter, t_env *env)
{
	(void)delimiter;
	(void)env;
	return (NULL);
}

// Non utilisé
int	open_heredoc_file(char *filename)
{
	(void)filename;
	return (-1);
}

/* ========================= UTILITAIRES NON UTILISÉS ========================= */

// Non utilisé
char	*expand_variables(char *str, t_shell *shell)
{
	(void)str;
	(void)shell;
	return (NULL);
}

// Non utilisé
char	*create_prompt(int last_exit_status)
{
	(void)last_exit_status;
	return (NULL);
}

// Non utilisé
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	(void)ptr;
	(void)old_size;
	(void)new_size;
	return (NULL);
}

// Non utilisé
int	execute_external_command(char **args, t_env *env)
{
	(void)args;
	(void)env;
	return (0);
}

// Non utilisé
int	tokenize_line(char *line, t_token **tokens)
{
	(void)line;
	(void)tokens;
	return (0);
}

// Non utilisé - stub déjà dans missing_utils.c
//int	parse_ast(t_token *tokens, t_ast_node **ast)

/* ========================= FONCTIONS EXPORT NON UTILISÉES ========================= */

// Non utilisé
char	**env_to_sorted_array(t_env *env)
{
	(void)env;
	return (NULL);
}

// Non utilisé
int	ft_export(t_env *env)
{
	(void)env;
	return (0);
}

// Non utilisé
t_env	*get_env(t_env *env, const char *key)
{
	(void)env;
	(void)key;
	return (NULL);
}

// Non utilisé
int	set_env_export_only(t_env **my_env, const char *key)
{
	(void)my_env;
	(void)key;
	return (0);
}

/* ========================= BUILTIN NON UTILISÉ ========================= */

// Remplacé par ft_env_custom
int	ft_env(char **env)
{
	(void)env;
	return (0);
}
