/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (1);
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line[i] == '\0');
}

static void	process_command_line(t_simple_data *data)
{
	int	i;

	add_history(data->line);
	data->args = simple_tokenize(data->line);
	if (data->args)
	{
		i = 0;
		while (data->args[i])
		{
			char *expanded = expand_and_parse_token(data->args[i], data->env);
			free(data->args[i]);
			data->args[i] = expanded;
			i++;
		}
		simple_execute(data);
		free_args(data->args);
	}
}

void	simple_minishell_loop(t_env *env)
{
	t_simple_data	data;
	
	data.env = env;
	init_signals_prompt();
	while (1)
	{
		data.line = readline("minishell$ ");
		if (!data.line)
		{
			printf("exit\n");
			break;
		}
		if (is_empty_line(data.line))
		{
			free(data.line);
			continue;
		}
		process_command_line(&data);
		free(data.line);
	}
}
