/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/18 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fonction pour libérer un tableau de chaînes (récupérée de pipe_utils)
void	ft_free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

// Fonction de nettoyage des fichiers heredoc (version simplifiée)
void	cleanup_heredoc_files(void)
{
	// Dans l'architecture simplifiée, les fichiers heredoc sont nettoyés directement
	// Cette fonction est un stub pour éviter les erreurs de linkage
}

// Stub pour parse_ast_new (remplacé par l'architecture simple)
t_ast_node	*parse_ast_new(t_token *tokens)
{
	(void)tokens;
	return (NULL);
}

// Stub pour check_syntax (remplacé par validation simplifiée)
int	check_syntax(t_token *tokens)
{
	(void)tokens;
	return (1); // Toujours valide dans l'architecture simple
}

// Stub pour tokenizer (remplacé par simple_tokenize)
t_token	*tokenizer(char *line)
{
	(void)line;
	return (NULL); // Pas utilisé dans l'architecture simple
}
