/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>

/* macOS compatibility for WEXITCODE */
#ifndef WEXITCODE
# define WEXITCODE(status) WEXITSTATUS(status)
#endif

/* Exit codes */
# define SUCCESS 0
# define FAILURE 1
# define ERROR 2

/* Token types */
typedef enum e_token_type
{
	WORD,
	PIPE,
	R_IN,
	R_OUT,
	R_APPEND,
	HEREDOC,
	AND,
	OR,
	SEMICOLON,
	LPAREN,
	RPAREN
}	e_token_type;

/* Quote types */
typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	e_quote_type;

/* Token structure */
typedef struct s_token
{
	e_token_type		type;
	char				*value;
	e_quote_type		quote_type;
	int					has_space_before;
	struct s_token		*next;
}	t_token;

/* Token data for creation */
typedef struct s_token_data
{
	char			*value;
	e_token_type	type;
	e_quote_type	quote_type;
	int				has_space_before;
}	t_token_data;

/* Forward declarations */
typedef struct s_ast_node	t_ast_node;

/* Redirection structure */
typedef struct s_redirect
{
	int					type;
	char				*file;
	int					fd;
	struct s_redirect	*next;
}	t_redirect;

/* Environment variable structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

/* Command structure */
typedef struct s_command
{
	char				**argv;
	struct s_redir		*redirs;
	struct s_command	*next;
}	t_command;

/* Redirection types */
typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	e_redir_type;

/* Redirection structure */
typedef struct s_redir
{
	e_redir_type		type;
	char				*file;
	struct s_redir		*next;
}	t_redir;

/* Main shell structure */
typedef struct s_shell
{
	char		**envp;
	t_env		*env_list;
	t_token		*tokens;
	t_ast_node	*ast_root;
	int			last_exit_status;
	int			exit_code;
	int			stdin_backup;
	int			stdout_backup;
	pid_t		*pids;
	int			pid_count;
}	t_shell;

/* AST node types */
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_SEQUENCE
}	e_ast_type;

/* Simple command structure for AST */
typedef struct s_simple_cmd
{
	char				**argv;
	struct s_redir		*redirs;
}	t_simple_cmd;

/* AST node structure */
typedef struct s_ast_node
{
	e_ast_type			type;
	union {
		t_simple_cmd		*cmd;
		struct {
			struct s_ast_node	*left;
			struct s_ast_node	*right;
		} binary;
		struct s_ast_node	*child;
	} data;
}	t_ast_node;

/* Global variable for signal handling */
extern int	g_exit_status;

/* Function prototypes */

/* Main functions */
int		main(int argc, char **argv, char **envp);
void	minishell_loop(t_env **my_env, char **envp);
void	shell_loop(t_shell *shell);
void	process_input(char *line, t_env **my_env, char **env);

/* Simple shell functions */
void	simple_minishell_loop(t_env *env);
void	launch_extern_command_simple(char **args, t_env *env);

/* Tokenizer functions */
t_token		*tokenizer(char *input);
t_token		*new_token(char *value, e_token_type type, e_quote_type quote_type, int has_space_before);
void		add_token(t_token **head, t_token_data data);
int			handle_token(char *input, int *i, t_token **tokens, int *has_space);

/* Parsing functions */
t_command	*parse_commands(t_token *tokens);
void		expand_tokens(t_token *tokens, t_env *env);
void		merge_token(t_token **tokens);
void		strip_quotes_inplace(t_token *tokens);
char		*strip_quotes(char *str);
int			count_token_args(t_token *token);
t_command	*build_command(t_token **tokens);
int			fill_command_argv_and_redirs(t_command *cmd, t_token **tok);

/* Path and execution utilities */
char		*find_command_path(char *cmd, t_env *env);
char		*get_path(char *cmd, char *envp[]);
char		**split_path(char *envp[]);
char		*get_env_path(char *env[]);
void		ft_free_tab(char **tab);
void		cleanup_argv(char **argv, int count);

/* Execution functions */
int		execute_ast(t_ast_node *node, t_env **env);
int		execute_single_command(t_ast_node *node, t_env **env);
int		execute_builtin_cmd(char **args, t_env **env);
int		builtin_help(void);
void	setup_redirections(t_redirect *redirects, t_shell *shell);

/* Built-in functions */
int		is_builtin_cmd(char *cmd);

/* Environment functions */
t_env	*init_env(t_env **env, char **envp);
t_env	*create_env_node(const char *key, const char *value);
int		set_env(t_env **env, const char *key, const char *value);
char	*get_env_value(t_env *env_list, char *key);
int		set_env_value(t_env **env_list, char *key, char *value);
int		unset_env_value(t_env **env_list, char *key);
char	**env_to_array(t_env *env_list);
void	free_env(t_env *env);

/* Utility functions */
char	*expand_variables(char *str, t_shell *shell);
void	print_error(char *cmd, char *msg);
char	**dup_args(char **args);
void	free_args(char **args);
void	free_tokens(t_token *head);
void	free_ast(t_ast_node *node);
void	free_command_list(t_command *cmd);
void	cleanup_all(t_token *tokens, t_command *cmd_list, char *line, t_env **env);

/* Signal handling */
void	setup_child_signals(void);

/* Advanced builtin functions */
int		ft_cd(char **argv, t_env **env);
void	ft_exit(char **argv, t_env **env, t_token *tokens, t_command *cmd_list);
int		ft_pwd(char **argv);
int		ft_echo(char **argv);
int		handle_builtin(char **argv, t_env **my_env);
int		check_multiple_n(char *str);
int		ft_echo_n(char **argv);
void	builtin(t_command *cmd, t_env **my_env, char **env, t_token *tokens);

/* Environment expansion functions */
char	*get_variable_value_from_env(t_env *my_env, char *var_name);
char	*get_variable_value(char *input, int *i, t_env *my_env);
void	append_str(const char *str, char **output);
void	append_char(char c, char **output);
void	handle_dollar(char *input, int *i, char **output, t_env *my_env);
char	*expand_token(t_token *token, t_env *my_env);

/* Export utilities */
int		env_size(t_env *env);
char	*ft_strjoin_free(char *s1, const char *s2);
void	ft_sort_str_array(char **arr);
char	**env_to_sorted_array(t_env *env);
int		ft_export(t_env *env);
t_env	*get_env(t_env *env, const char *key);
int		set_env_export_only(t_env **my_env, const char *key);
int		handle_export(char **argv, t_env **my_env);

/* Environment builtin functions */
int		ft_env(char **env);
int		ft_env_custom(t_env *env);
int		ft_unset(t_env **my_env, const char *key);

/* AST functions */
t_ast_node	*create_ast_node(e_ast_type type);
t_simple_cmd	*command_to_simple_cmd(t_command *cmd);
t_ast_node	*parse_simple_command(t_token **tokens);
t_ast_node	*parse_pipe(t_token **tokens);
t_ast_node	*parse_and(t_token **tokens);
t_ast_node	*parse_or(t_token **tokens);
t_ast_node	*parse_sequence(t_token **tokens);
t_ast_node	*parse_ast_new(t_token *tokens);

/* AST execution functions */
int		execute_simple_command(t_simple_cmd *cmd, t_env **env);
int		execute_pipe(t_ast_node *left, t_ast_node *right, t_env **env);
int		execute_ast_new(t_ast_node *node, t_env **env);
int		execute_simple_command(t_simple_cmd *cmd, t_env **env);
/* AST memory management */
void	free_simple_cmd(t_simple_cmd *cmd);
void	free_ast_new(t_ast_node *node);
int		process_heredocs(t_command *cmd, t_env *env);
int		process_heredocs_simple(t_simple_cmd *cmd, t_env *env);

/* Redirection functions */
int		apply_redirection(t_redir *redir);
int		apply_redirections(t_redir *redirs);
void	save_std_fds(int *stdin_backup, int *stdout_backup);
void	restore_std_fds(int stdin_backup, int stdout_backup);

/* Robust memory management functions */
void	free_tokens_safe(t_token **tokens);
void	free_ast_safe(t_ast_node **root);
void	free_string_array(char **array);
void	free_env_list_safe(t_env **env_list);
void	error_exit_safe(t_shell *shell, char *message, int exit_code);
void	*safe_malloc(size_t size);
void	*safe_realloc(void *ptr, size_t new_size);
char	*safe_strdup(const char *str);
char	*safe_strjoin(const char *s1, const char *s2);
void	cleanup_on_signal(t_shell *shell);

/* Robust heredoc functions */
char	*handle_heredoc_robust(char *delimiter, t_env *env);
void	cleanup_heredoc_files(void);
int		open_heredoc_file(char *filename);

/* Signal handling functions (simple approach like friend's code) */
void	init_signals_prompt(void);
void	handle_status_and_print(int status);
void	reset_signals_to_default(void);
void	ignore_sigint(void);
void	hd_set_signals(struct sigaction *old_int);
void	hd_restore_signals(const struct sigaction *old_int);
void	disable_echoctl(void);
void	enable_echoctl(void);

/* Utility functions for robust version */
int		is_empty_line(char *line);
char	*create_prompt(int last_exit_status);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_env *env);
int		execute_external_command(char **args, t_env *env);
int		tokenize_line(char *line, t_token **tokens);
int		check_syntax(t_token *tokens);
int		parse_ast(t_token *tokens, t_ast_node **ast);

#endif
