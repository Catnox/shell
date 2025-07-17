/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, e_token_type type, e_quote_type quote_type,
		int has_space_before)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->quote_type = quote_type;
	token->has_space_before = has_space_before;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token_data data)
{
	t_token	*new;
	t_token	*tmp;

	new = new_token(data.value, data.type,
			data.quote_type, data.has_space_before);
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == ';' || c == '(' || c == ')')
		return (1);
	return (0);
}

void	parse_unquoted_word(char *input, int *i,
		t_token **tokens, int has_space)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& !is_operator(input[*i]) && input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		word = ft_strndup(&input[start], *i - start);
		add_token(tokens, (t_token_data){word, WORD, NO_QUOTE, has_space});
		free(word);
	}
}

int	parse_quoted_word(char *input, int *i,
		t_token **tokens, int has_space)
{
	int				start;
	char			quote_char;
	e_quote_type	quote_type;
	char			*word;
	char			*raw;

	quote_char = input[*i];
	if (quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!(input[*i] == quote_char))
		return (printf("Syntax error: unclosed %c-quote\n", quote_char), 0);
	raw = ft_strndup(&input[start], *i - start);
	word = ft_strtrim(raw, " \t\n\r\v\f");
	add_token(tokens, (t_token_data){word, WORD, quote_type, has_space});
	free(word);
	free(raw);
	(*i)++;
	return (1);
}

int	handle_word(char *input, int *i, t_token **tokens, int has_space)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (parse_quoted_word(input, i, tokens, has_space));
	else
	{
		parse_unquoted_word(input, i, tokens, has_space);
		return (1);
	}
}

void	handle_operator(char *input, int *i,
		t_token **tokens, int has_space)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, (t_token_data){"<<", HEREDOC, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, (t_token_data){">>", R_APPEND, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		add_token(tokens, (t_token_data){"&&", AND, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '|' && input[*i + 1] == '|')
	{
		add_token(tokens, (t_token_data){"||", OR, NO_QUOTE, has_space});
		*i += 2;
	}
	else if (input[*i] == '|')
	{
		add_token(tokens, (t_token_data){"|", PIPE, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == ';')
	{
		add_token(tokens, (t_token_data){";", SEMICOLON, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == '(')
	{
		add_token(tokens, (t_token_data){"(", LPAREN, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == ')')
	{
		add_token(tokens, (t_token_data){")", RPAREN, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, (t_token_data){"<", R_IN, NO_QUOTE, has_space});
		(*i)++;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, (t_token_data){">", R_OUT, NO_QUOTE, has_space});
		(*i)++;
	}
}

int	handle_token(char *input, int *i, t_token **tokens, int *has_space)
{
	if (ft_isspace(input[*i]))
	{
		(*has_space) = 1;
		(*i)++;
	}
	else if (is_operator(input[*i]))
	{
		handle_operator(input, i, tokens, *has_space);
		(*has_space) = 0;
	}
	else
	{
		if (!handle_word(input, i, tokens, *has_space))
			return (0);
		(*has_space) = 0;
	}
	return (1);
}

t_token	*tokenizer(char *input)
{
	t_token	*tokens;
	int		i;
	int		has_space;

	i = 0;
	has_space = 1;
	tokens = NULL;
	while (input[i])
	{
		if (!handle_token(input, &i, &tokens, &has_space))
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
