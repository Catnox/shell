# Fonctions non utilisées identifiées

## Fonctions principales non utilisées
- `minishell_loop` - remplacée par `simple_minishell_loop`
- `shell_loop` - architecture AST abandonnée 
- `process_input` - remplacée par l'architecture simple

## Fonctions de parsing non utilisées (architecture AST)
- `parse_commands` - remplacé par parsing simple
- `merge_token` - non utilisé
- `strip_quotes_inplace` - non utilisé  
- `build_command` - non utilisé
- `fill_command_argv_and_redirs` - non utilisé

## Utilitaires de chemin non utilisés
- `get_path` - remplacé par `find_command_path`
- `split_path` - non utilisé
- `get_env_path` - non utilisé

## Fonctions d'exécution AST non utilisées
- `execute_ast` - architecture AST abandonnée
- `execute_single_command` - remplacée par architecture simple
- `execute_builtin_cmd` - remplacée par `handle_builtin`
- `setup_redirections` - non utilisée

## Fonctions AST non utilisées
- `create_ast_node` - architecture AST abandonnée
- `command_to_simple_cmd` - non utilisé
- `parse_simple_command` - non utilisé
- `parse_pipe` - non utilisé
- `parse_and` - non utilisé  
- `parse_or` - non utilisé
- `parse_sequence` - non utilisé

## Fonctions d'exécution AST non utilisées
- `execute_pipe` - remplacée par `execute_pipe_chain`
- `execute_ast_new` - non utilisé
- `free_simple_cmd` - non utilisé
- `free_ast_new` - non utilisé
- `process_heredocs` - non utilisé
- `process_heredocs_simple` - non utilisé

## Fonctions de redirection non utilisées
- `apply_redirection` - remplacées par l'architecture simple
- `apply_redirections` - remplacées par l'architecture simple
- `save_std_fds` - non utilisées
- `restore_std_fds` - non utilisées

## Fonctions robustes non utilisées
- `free_tokens_safe` - non utilisé
- `free_ast_safe` - non utilisé  
- `free_string_array` - non utilisé
- `free_env_list_safe` - non utilisé
- `error_exit_safe` - non utilisé
- `safe_malloc` - non utilisé
- `safe_realloc` - non utilisé
- `safe_strdup` - non utilisé
- `safe_strjoin` - non utilisé
- `cleanup_on_signal` - non utilisé

## Fonctions heredoc robustes non utilisées  
- `handle_heredoc_robust` - non utilisé
- `open_heredoc_file` - non utilisé

## Utilitaires non utilisés
- `expand_variables` - non utilisé
- `create_prompt` - non utilisé
- `ft_realloc` - non utilisé
- `execute_external_command` - non utilisé
- `tokenize_line` - non utilisé
- `parse_ast` - non utilisé

## Fonctions export non utilisées
- `env_to_sorted_array` - non utilisé
- `ft_export` - non utilisé
- `set_env_export_only` - non utilisé
- `get_env` - non utilisé

## Builtin non utilisé
- `ft_env` - remplacé par `ft_env_custom`

Date: 19 juillet 2025
Raison: Migration vers architecture simple, abandon de l'AST complexe
