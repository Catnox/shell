/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*strip_quotes(char *str)
{
	size_t	len;

	len = 0;
	if (!str)
		return (NULL);
	while (str[len])
		len++;
	if ((str[0] == '\'' && str[len - 1] == '\'')
		|| (str[0] == '"' && str[len - 1] == '"'))
		return (ft_strndup(str + 1, len - 2));
	return (ft_strdup(str));
}

void	strip_quotes_inplace(t_token *tokens)
{
	char	*stripped;

	while (tokens)
	{
		stripped = strip_quotes(tokens->value);
		free(tokens->value);
		tokens->value = stripped;
		tokens = tokens->next;
	}
}
