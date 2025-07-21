/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/21 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hd_is_end(char *line, char *delim)
{
	char	*nl;

	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = '\0';
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

void	hd_write(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

void	reset_buffer_state(int *buffer_pos, int *buffer_size)
{
	*buffer_pos = 0;
	*buffer_size = 0;
}

char	*init_simple_line(int *line_len)
{
	char	*line;

	line = allocate_line_buffer();
	if (!line)
		return (NULL);
	*line_len = 0;
	return (line);
}

char	*get_heredoc_line(int is_interactive)
{
	char	*line;

	if (is_interactive)
		line = read_line_interactive();
	else
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_simple(STDIN_FILENO);
	}
	return (line);
}
