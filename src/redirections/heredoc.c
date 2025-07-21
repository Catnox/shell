/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/19 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_line_simple(int fd)
{
	static char	buffer[4096];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;

	if (buffer_size < 0)
		reset_buffer_state(&buffer_pos, &buffer_size);
	line = init_simple_line(&line_len);
	if (!line)
		return (NULL);
	if (!process_line_reading(fd, buffer, &buffer_pos, &buffer_size, 
			line, &line_len))
	{
		reset_buffer_state(&buffer_pos, &buffer_size);
		free(line);
		return (NULL);
	}
	finalize_line(line, line_len);
	return (line);
}

static int	check_delimiter_match(char *line, char *delim, int is_interactive)
{
	if ((is_interactive && ft_strcmp(line, delim) == 0) ||
		(!is_interactive && hd_is_end(line, delim)))
		return (1);
	return (0);
}

static int	process_heredoc_line(char *line, char *delim, int fd, int is_interactive)
{
	if (g_exit_status == 130)
	{
		if (line)
			free(line);
		return (-1);
	}
	if (!line)
		return (1);
	if (check_delimiter_match(line, delim, is_interactive))
	{
		free(line);
		return (2);
	}
	hd_write(fd, line);
	free(line);
	return (0);
}

static int	heredoc_loop(char *delim, int fd)
{
	char	*line;
	int		is_interactive;
	int		result;

	is_interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (g_exit_status == 130)
			return (-1);
		line = get_heredoc_line(is_interactive);
		result = process_heredoc_line(line, delim, fd, is_interactive);
		if (result == -1)
			return (-1);
		if (result == 1)
			return (1);
		if (result == 2)
			break;
	}
	return (0);
}

char	*handle_heredoc(char *delimiter)
{
	char				*tmp_filename;
	int					tmp_fd;
	struct sigaction	old_int;
	int					saved;
	int					ret;

	tmp_fd = create_tmp_heredoc_file(&tmp_filename);
	if (tmp_fd == -1)
		return (NULL);
	setup_heredoc_signals(&old_int, &saved);
	ret = heredoc_loop(delimiter, tmp_fd);
	cleanup_heredoc_signals(&old_int, saved, ret, delimiter);
	close(tmp_fd);
	if (should_cleanup_file(ret))
	{
		unlink(tmp_filename);
		free(tmp_filename);
		return (NULL);
	}
	return (tmp_filename);
}

