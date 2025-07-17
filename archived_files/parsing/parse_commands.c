/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_commands.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_token_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		else if (token->type >= R_IN && token->type <= HEREDOC)
			token = token->next;
		if (token)
			token = token->next;
	}
	return (count);
}

e_redir_type	map_token_to_redir_type(e_token_type token_type)
{
	if (token_type == R_IN)
		return (REDIR_IN);
	else if (token_type == R_OUT)
		return (REDIR_OUT);
	else if (token_type == R_APPEND)
		return (REDIR_APPEND);
	else if (token_type == HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_IN);
}

int	handle_redirection(t_command *cmd,
		t_token **tok, t_redir **last_redir)
{
	e_redir_type	rtype;
	t_redir			*r;

	rtype = map_token_to_redir_type((*tok)->type);
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != WORD)
		return (0);
	r = malloc(sizeof(t_redir));
	if (!r)
		return (0);
	r->type = rtype;
	r->file = ft_strdup((*tok)->value);
	if (!r->file)
	{
		free(r);
		return (0);
	}
	r->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = r;
	else
		(*last_redir)->next = r;
	*last_redir = r;
	return (1);
}

int	fill_argv_loop(t_command *cmd, t_token **tok,
		char **argv, t_redir **last_redir)
{
	int	i;

	i = 0;
	while (*tok && (*tok)->type != PIPE)
	{
		if ((*tok)->type == WORD)
		{
			argv[i] = ft_strdup((*tok)->value);
			if (!argv[i])
				return (cleanup_argv(argv, i), 0);
			i++;
		}
		else if ((*tok)->type >= R_IN && (*tok)->type <= HEREDOC)
		{
			if (!handle_redirection(cmd, tok, last_redir))
				return (cleanup_argv(argv, i), 0);
		}
		*tok = (*tok)->next;
	}
	argv[i] = NULL;
	cmd->argv = argv;
	return (1);
}

int	fill_command_argv_and_redirs(t_command *cmd, t_token **tok)
{
	t_redir	*last_redir;
	int		argc;
	char	**argv;

	last_redir = NULL;
	argc = count_token_args(*tok);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (0);
	return (fill_argv_loop(cmd, tok, argv, &last_redir));
}

t_command	*build_command(t_token **tokens)
{
	t_command	*cmd;
	t_token		*tok;

	tok = *tokens;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	if (!fill_command_argv_and_redirs(cmd, &tok))
	{
		free(cmd);
		return (NULL);
	}
	*tokens = tok;
	return (cmd);
}

t_command	*parse_commands(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;

	current = NULL;
	head = NULL;
	while (tokens)
	{
		cmd = build_command(&tokens);
		if (!cmd)
			return (NULL);
		if (!head)
			head = cmd;
		else
			current->next = cmd;
		current = cmd;
		if (tokens && tokens->type == PIPE)
			tokens = tokens->next;
	}
	return (head);
}
