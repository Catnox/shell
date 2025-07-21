/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_interactive.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/21 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_signal_interruption(char *line)
{
	if (g_exit_status == 130)
	{
		free(line);
		return (1);
	}
	return (0);
}

static char	*resize_buffer_if_needed(char *line, int len, int *capacity)
{
	char	*new_line;

	if (len >= *capacity - 1)
	{
		*capacity *= 2;
		new_line = realloc(line, *capacity);
		if (!new_line)
			return (NULL);
		return (new_line);
	}
	return (line);
}

static int	handle_special_chars(int c, int len, char *line)
{
	if (c == EOF)
	{
		if (len == 0)
		{
			free(line);
			return (-1);
		}
		return (1);
	}
	if (c == '\n')
		return (1);
	return (0);
}

static int	process_input_char(int c, char **line, int *len, int *capacity)
{
	int	special_result;

	special_result = handle_special_chars(c, *len, *line);
	if (special_result == -1)
		return (-1);
	if (special_result == 1)
		return (1);
	*line = resize_buffer_if_needed(*line, *len, capacity);
	if (!*line)
		return (-1);
	(*line)[(*len)++] = c;
	return (0);
}

char	*read_line_interactive(void)
{
	char	*line;
	int		capacity = 128;
	int		len = 0;
	int		c, result;

	line = init_line_buffer();
	if (!line)
		return (NULL);
	while (1)
	{
		if (check_signal_interruption(line))
			return (NULL);
		c = getchar();
		if (check_signal_interruption(line))
			return (NULL);
		result = process_input_char(c, &line, &len, &capacity);
		if (result == -1)
			return (NULL);
		if (result == 1)
			break;
	}
	line[len] = '\0';
	return (line);
}
