/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user <user@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 00:00:00 by user              #+#    #+#             */
/*   Updated: 2025/07/16 00:00:00 by user             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_simple_cmd(t_simple_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	
	if (cmd->argv)
	{
		i = 0;
		while (cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
	}
	
	// TODO: Libérer les redirections si nécessaire
	// free_redirections(cmd->redirs);
	
	free(cmd);
}

void	free_ast_new(t_ast_node *node)
{
	if (!node)
		return ;
	
	switch (node->type)
	{
		case AST_COMMAND:
			free_simple_cmd(node->data.cmd);
			break;
			
		case AST_SUBSHELL:
			free_ast_new(node->data.child);
			break;
			
		case AST_PIPE:
		case AST_AND:
		case AST_OR:
		case AST_SEQUENCE:
			free_ast_new(node->data.binary.left);
			free_ast_new(node->data.binary.right);
			break;
	}
	
	free(node);
}
