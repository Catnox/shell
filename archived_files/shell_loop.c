/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_prompt(t_shell *shell)
{
	(void)shell; // Suppress unused parameter warning
	return (ft_strdup("minishell$ "));
}

// Execute a list of commands with redirections support
int	execute_command_list(t_command *commands, t_env **env)
{
	t_command	*current;
	t_simple_cmd	simple_cmd;
	int			result;

	current = commands;
	result = 0;
	
	while (current)
	{
		// Convert t_command to t_simple_cmd for compatibility
		simple_cmd.argv = current->argv;
		simple_cmd.redirs = current->redirs;
		
		// Execute this command
		result = execute_simple_command(&simple_cmd, env);
		
		current = current->next;
	}
	
	return (result);
}

void	process_input(char *line, t_env **my_env, char **env)
{
	t_token		*tokens;
	t_command	*commands;

	if (!line || !*line)
		return ;

	// Add to history if not empty
	if (ft_strlen(line) > 0)
		add_history(line);

	// Tokenize input
	tokens = tokenizer(line);
	if (!tokens)
	{
		g_exit_status = FAILURE;
		return ;
	}

	// Parse tokens into commands (this handles redirections)
	commands = parse_commands(tokens);
	free_tokens(tokens);
	
	if (!commands)
	{
		g_exit_status = FAILURE;
		return ;
	}

	// Execute commands directly (without AST for now)
	g_exit_status = execute_command_list(commands, my_env);
	free_command_list(commands);

	(void)env; // Suppress unused parameter warning
}

void	shell_loop(t_shell *shell)
{
	char	*input;
	char	*prompt;

	while (1)
	{
		// Configure les signaux pour le mode interactif à chaque itération (comme votre ami)
		init_signals_prompt();
		
		// Get and display prompt
		prompt = get_prompt(shell);
		input = readline(prompt);
		free(prompt);

		// Handle EOF (Ctrl+D)
		if (!input)
		{
			printf("exit\n");
			break ;
		}

		// Si ligne vide, continuer
		if (!*input)
		{
			free(input);
			continue;
		}

		// Process the input
		process_input(input, &shell->env_list, shell->envp);
		free(input);
	}
}
