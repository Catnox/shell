/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/16 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Crée un nouveau nœud AST
t_ast_node *create_ast_node(e_ast_type type)
{
    t_ast_node *node = malloc(sizeof(t_ast_node));
    if (!node)
        return (NULL);
    node->type = type;
    if (type == AST_COMMAND)
        node->data.cmd = NULL;
    else if (type == AST_SUBSHELL)
        node->data.child = NULL;
    else
    {
        node->data.binary.left = NULL;
        node->data.binary.right = NULL;
    }
    return (node);
}

// Convertit une t_command en t_simple_cmd
t_simple_cmd *command_to_simple_cmd(t_command *cmd)
{
    t_simple_cmd *simple_cmd = malloc(sizeof(t_simple_cmd));
    if (!simple_cmd)
        return (NULL);
    
    simple_cmd->argv = cmd->argv;
    simple_cmd->redirs = cmd->redirs;
    
    return (simple_cmd);
}

// Parse une commande simple
t_ast_node *parse_simple_command(t_token **tokens)
{
    t_ast_node *node = create_ast_node(AST_COMMAND);
    if (!node)
        return (NULL);
    
    // Utiliser votre logique existante pour parser une commande
    t_command *cmd = build_command(tokens);
    if (!cmd)
    {
        free(node);
        return (NULL);
    }
    
    // Convertir t_command en t_simple_cmd
    node->data.cmd = command_to_simple_cmd(cmd);
    if (!node->data.cmd)
    {
        free(node);
        return (NULL);
    }
    
    // Libérer la structure t_command (mais pas son contenu qui est maintenant dans simple_cmd)
    free(cmd);
    
    return (node);
}

// Parse les pipes (priorité la plus haute)
t_ast_node *parse_pipe(t_token **tokens)
{
    t_ast_node *left = parse_simple_command(tokens);
    
    while (*tokens && (*tokens)->type == PIPE)
    {
        *tokens = (*tokens)->next; // Consommer le PIPE
        
        t_ast_node *pipe_node = create_ast_node(AST_PIPE);
        pipe_node->data.binary.left = left;
        pipe_node->data.binary.right = parse_simple_command(tokens);
        
        left = pipe_node;
    }
    
    return (left);
}

// Parse les opérateurs AND (priorité moyenne)
t_ast_node *parse_and(t_token **tokens)
{
    t_ast_node *left = parse_pipe(tokens);
    
    while (*tokens && (*tokens)->type == AND)
    {
        *tokens = (*tokens)->next; // Consommer le AND
        
        t_ast_node *and_node = create_ast_node(AST_AND);
        and_node->data.binary.left = left;
        and_node->data.binary.right = parse_pipe(tokens);
        
        left = and_node;
    }
    
    return (left);
}

// Parse les opérateurs OR (priorité faible)
t_ast_node *parse_or(t_token **tokens)
{
    t_ast_node *left = parse_and(tokens);
    
    while (*tokens && (*tokens)->type == OR)
    {
        *tokens = (*tokens)->next; // Consommer le OR
        
        t_ast_node *or_node = create_ast_node(AST_OR);
        or_node->data.binary.left = left;
        or_node->data.binary.right = parse_and(tokens);
        
        left = or_node;
    }
    
    return (left);
}

// Parse les séquences (priorité la plus faible)
t_ast_node *parse_sequence(t_token **tokens)
{
    t_ast_node *left = parse_or(tokens);
    
    while (*tokens && (*tokens)->type == SEMICOLON)
    {
        *tokens = (*tokens)->next; // Consommer le SEMICOLON
        
        t_ast_node *seq_node = create_ast_node(AST_SEQUENCE);
        seq_node->data.binary.left = left;
        seq_node->data.binary.right = parse_or(tokens);
        
        left = seq_node;
    }
    
    return (left);
}

// Fonction principale de parsing AST
t_ast_node *parse_ast_new(t_token *tokens)
{
    if (!tokens)
        return (NULL);
    
    t_token *current = tokens;
    
    // Maintenant on gère les pipes
    return (parse_pipe(&current));
}
