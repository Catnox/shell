/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Forward declaration
t_ast_node	*parse_tokens_to_ast(t_token *tokens);

// Fonction d'interface pour la compatibilité
int	parse_ast(t_token *tokens, t_ast_node **ast)
{
	if (!tokens || !ast)
		return (0);
	
	*ast = parse_tokens_to_ast(tokens);
	return (*ast != NULL);
}

// Parser principal compatible avec la nouvelle architecture
t_ast_node	*parse_tokens_to_ast(t_token *tokens)
{
	t_ast_node	*node;
	t_simple_cmd	*cmd;

	if (!tokens)
		return (NULL);
	
	// Allocation sécurisée du noeud
	node = safe_malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	
	// Allocation de la commande simple
	cmd = safe_malloc(sizeof(t_simple_cmd));
	if (!cmd)
	{
		free(node);
		return (NULL);
	}
	
	// Configuration pour commande simple
	node->type = AST_COMMAND;
	node->data.cmd = cmd;
	
	// Allocation des arguments
	cmd->argv = safe_malloc(sizeof(char *) * 2);
	if (!cmd->argv)
	{
		free(cmd);
		free(node);
		return (NULL);
	}
	
	cmd->argv[0] = safe_strdup(tokens->value);
	cmd->argv[1] = NULL;
	cmd->redirs = NULL;
	
	return (node);
}

// Fonction de libération utilisant le memory manager robuste
void	free_ast(t_ast_node *node)
{
	if (!node)
		return;
	
	// Utiliser la fonction robuste avec l'adresse
	free_ast_safe(&node);
}
