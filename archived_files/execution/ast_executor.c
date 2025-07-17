/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/16 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Exécute une commande simple (version modifiée avec execve)
int execute_simple_command(t_simple_cmd *cmd, t_env **env)
{
    char *cmd_path;
    char **env_array;
    char **path_array;
    
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return (1);
    
    // Traiter les heredocs avant l'exécution
    if (process_heredocs_simple(cmd, *env) < 0)
        return (1);
    
    // Gestion des builtins avec redirections
    if (is_builtin(cmd->argv[0]))
    {
        int stdin_backup, stdout_backup;
        int result;
        
        // Sauvegarder les descripteurs standard
        save_std_fds(&stdin_backup, &stdout_backup);
        
        // Appliquer les redirections
        if (apply_redirections(cmd->redirs) < 0)
        {
            restore_std_fds(stdin_backup, stdout_backup);
            return (1);
        }
        
        // Exécuter le builtin
        result = handle_builtin(cmd->argv, env);
        
        // Restaurer les descripteurs standard
        restore_std_fds(stdin_backup, stdout_backup);
        
        return (result);
    }
    
    // Créer un tableau d'environnement pour get_path
    path_array = env_to_sorted_array(*env);
    if (!path_array)
        return (1);
    
    cmd_path = get_path(cmd->argv[0], path_array);
    if (!cmd_path)
    {
        printf("minishell: %s: command not found\n", cmd->argv[0]);
        ft_free_tab(path_array);
        return (127);
    }
    
    // Réutiliser le même tableau pour execve
    env_array = path_array;
    
    // Exécuter commande externe avec execve
    pid_t pid = fork();
    if (pid == 0)
    {
        // Processus enfant - appliquer les redirections
        if (apply_redirections(cmd->redirs) < 0)
            exit(1);
        
        execve(cmd_path, cmd->argv, env_array);
        perror("execve");
        exit(127);
    }
    else if (pid > 0)
    {
        // Processus parent
        int status;
        waitpid(pid, &status, 0);
        
        // Nettoyer
        free(cmd_path);
        ft_free_tab(env_array);
        
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
        return (1);
    }
    else
    {
        perror("fork");
        free(cmd_path);
        ft_free_tab(env_array);
        return (1);
    }
}

// Exécute un pipe
int execute_pipe(t_ast_node *left, t_ast_node *right, t_env **env)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;
    
    if (pipe(pipefd) == -1)
        return (1);
    
    pid1 = fork();
    if (pid1 == 0)
    {
        // Processus enfant 1 (commande de gauche)
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        int result = execute_ast_new(left, env);
        exit(result);
    }
    
    pid2 = fork();
    if (pid2 == 0)
    {
        // Processus enfant 2 (commande de droite)
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        int result = execute_ast_new(right, env);
        exit(result);
    }
    
    // Processus parent
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &status, 0);
    
    return (WEXITSTATUS(status));
}

// Fonction principale d'exécution AST
int execute_ast_new(t_ast_node *node, t_env **env)
{
    if (!node)
        return (0);
    
    switch (node->type)
    {
        case AST_COMMAND:
            return (execute_simple_command(node->data.cmd, env));
            
        case AST_PIPE:
            return (execute_pipe(node->data.binary.left, node->data.binary.right, env));
            
        case AST_AND:
            {
                int left_result = execute_ast_new(node->data.binary.left, env);
                if (left_result == 0) // Succès
                    return (execute_ast_new(node->data.binary.right, env));
                return (left_result);
            }
            
        case AST_OR:
            {
                int left_result = execute_ast_new(node->data.binary.left, env);
                if (left_result != 0) // Échec
                    return (execute_ast_new(node->data.binary.right, env));
                return (left_result);
            }
            
        case AST_SEQUENCE:
            execute_ast_new(node->data.binary.left, env);
            return (execute_ast_new(node->data.binary.right, env));
            
        case AST_SUBSHELL:
            // Fork et exécuter dans un sous-shell
            {
                pid_t pid = fork();
                if (pid == 0)
                {
                    int result = execute_ast_new(node->data.child, env);
                    exit(result);
                }
                int status;
                waitpid(pid, &status, 0);
                return (WEXITSTATUS(status));
            }
    }
    
    return (1);
}
