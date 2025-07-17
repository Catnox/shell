/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compatibility.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Stub pour expand_variables - utilise la fonction existante
char	*expand_variables(char *str, t_shell *shell)
{
	(void)shell;
	
	// Pour l'instant, retourne une copie simple
	// Remplace par la vraie fonction d'expansion si elle existe
	return (ft_strdup(str));
}

// Stub pour free_env_list
void	free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;
	
	current = env_list;
	while (current)
	{
		next = current->next;
		
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		
		free(current);
		current = next;
	}
}

// Stub pour parse - utilise le parser existant
t_ast_node	*parse(t_token *tokens)
{
	return (parse_ast_new(tokens));
}

// Stub pour tokenize - utilise le tokenizer existant
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	
	tokens = tokenizer(input);
	return (tokens);
}

// Implémentation de ft_realloc
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;
	
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	
	if (!ptr)
		return (malloc(new_size));
	
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	
	copy_size = old_size < new_size ? old_size : new_size;
	ft_memcpy(new_ptr, ptr, copy_size);
	
	free(ptr);
	return (new_ptr);
}
