/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_shell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Structure simplifiée comme votre ami
typedef struct s_simple_data
{
	char	*line;
	char	**tokens;
	char	**args;
	t_env	*env;
}	t_simple_data;

// Vérifier si ligne vide (simplifié de votre ami)
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

// Traiter un caractère dans l'expansion
static void	process_char_expansion(char *input, int *i, char **output, t_env *my_env, int *in_single_quotes, int *in_double_quotes)
{
	if (input[*i] == '\'' && !*in_double_quotes)
	{
		*in_single_quotes = !*in_single_quotes;
		(*i)++;
	}
	else if (input[*i] == '"' && !*in_single_quotes)
	{
		*in_double_quotes = !*in_double_quotes;
		(*i)++;
	}
	else if (input[*i] == '$' && !*in_single_quotes)
	{
		handle_dollar(input, i, output, my_env);
	}
	else
	{
		append_char(input[*i], output);
		(*i)++;
	}
}

// Expansion complète d'un token avec variables et guillemets
static char	*expand_and_parse_token(char *input, t_env *my_env)
{
	char	*output;
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	if (!input)
		return (NULL);
	output = NULL;
	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (input[i])
		process_char_expansion(input, &i, &output, my_env, &in_single_quotes, &in_double_quotes);
	if (!output)
		return (ft_strdup(""));
	return (output);
}

// Avancer dans un token en tenant compte des guillemets
static void	advance_in_token(char *line, int *i, int *in_quotes, char *quote_char)
{
	while (line[*i])
	{
		if ((line[*i] == '"' || line[*i] == '\'') && !*in_quotes)
		{
			*in_quotes = 1;
			*quote_char = line[*i];
		}
		else if (line[*i] == *quote_char && *in_quotes)
		{
			*in_quotes = 0;
			*quote_char = 0;
		}
		else if (!*in_quotes && (line[*i] == ' ' || line[*i] == '\t' || 
				line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
			break;
		(*i)++;
	}
}

// Traiter un token individuel
static void	process_token(char *line, int *i, int *count, int *in_quotes, char *quote_char)
{
	if (!*in_quotes && line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else if (!*in_quotes && line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (!*in_quotes && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
		(*i)++;
	else
		advance_in_token(line, i, in_quotes, quote_char);
	(*count)++;
}

// Compter le nombre de tokens
static int	count_tokens(char *line)
{
	int		i;
	int		count;
	int		in_quotes;
	char	quote_char;

	count = 0;
	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			in_quotes = 0;
			quote_char = 0;
			process_token(line, &i, &count, &in_quotes, &quote_char);
		}
	}
	return (count);
}

// Extraire un token à partir d'une position
static char	*extract_token(char *line, int *i)
{
	int		start;
	int		in_quotes;
	char	quote_char;

	start = *i;
	in_quotes = 0;
	quote_char = 0;
	
	if (!in_quotes && line[*i] == '<' && line[*i + 1] == '<')
		*i += 2;
	else if (!in_quotes && line[*i] == '>' && line[*i + 1] == '>')
		*i += 2;
	else if (!in_quotes && (line[*i] == '<' || line[*i] == '>' || line[*i] == '|'))
		(*i)++;
	else
		advance_in_token(line, i, &in_quotes, &quote_char);
	
	return (ft_substr(line, start, *i - start));
}

// Tokenisation améliorée avec parsing complet (divisée en sous-fonctions)
static char	**simple_tokenize(char *line)
{
	char	**tokens;
	int		i;
	int		count;
	char	*token;

	if (!line)
		return (NULL);
	
	count = count_tokens(line);
	tokens = malloc(sizeof(char *) * (count + 1));
	if (!tokens)
		return (NULL);
	
	count = 0;
	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i])
		{
			token = extract_token(line, &i);
			if (!token)
			{
				while (count > 0)
					free(tokens[--count]);
				free(tokens);
				return (NULL);
			}
			tokens[count++] = token;
		}
	}
	tokens[count] = NULL;
	return (tokens);
}

// Fonction pour créer un nom de fichier temporaire (copié de votre ami)
static char	*heredoc_tmp(void)
{
	static int	i = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(i++);
	filename = ft_strjoin("/tmp/heredoc_", num);
	if (!filename)
	{
		free(num);
		return (NULL);
	}
	free(num);
	return (filename);
}

// Créer fichier temporaire heredoc (copié de votre ami)
static int	create_tmp_heredoc_file(char **filename)
{
	*filename = heredoc_tmp();
	if (!*filename)
		return (-1);
	return (open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0600));
}

// Vérifier si c'est la fin du heredoc (copié de votre ami)
static int	hd_is_end(char *line, char *delim)
{
	char	*nl;

	nl = ft_strchr(line, '\n');
	if (nl)
		*nl = '\0';
	if (ft_strcmp(line, delim) == 0)
		return (1);
	return (0);
}

// Écrire ligne dans heredoc (copié de votre ami)
static void	hd_write(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

// Gérer le buffer pour la lecture
static int	handle_buffer_read(int fd, char *buffer, int *buffer_pos, int *buffer_size)
{
	if (*buffer_pos >= *buffer_size)
	{
		*buffer_size = read(fd, buffer, 4096);
		if (*buffer_size <= 0)
			return (0);
		*buffer_pos = 0;
	}
	return (1);
}

// Finaliser la ligne lue
static void	finalize_line(char *line, int line_len)
{
	if (line_len == 0)
	{
		line[0] = '\n';
		line[1] = '\0';
	}
	else
	{
		line[line_len] = '\n';
		line[line_len + 1] = '\0';
	}
}

// Lecture simple ligne par ligne (remplace get_next_line)
static char	*read_line_simple(int fd)
{
	static char	buffer[4096];
	static int	buffer_pos = 0;
	static int	buffer_size = 0;
	char		*line;
	int			line_len;

	line = malloc(1024);
	if (!line)
		return (NULL);
	line_len = 0;
	while (1)
	{
		if (!handle_buffer_read(fd, buffer, &buffer_pos, &buffer_size))
		{
			if (line_len == 0)
			{
				free(line);
				return (NULL);
			}
			break;
		}
		if (buffer[buffer_pos] == '\n')
		{
			buffer_pos++;
			break;
		}
		line[line_len++] = buffer[buffer_pos++];
		if (line_len >= 1023)
			break;
	}
	line[line_len] = '\0';
	finalize_line(line, line_len);
	return (line);
}

// Boucle principale heredoc (copié de votre ami)
static int	heredoc_loop(char *delim, int fd)
{
	char	*line;

	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = read_line_simple(STDIN_FILENO);
		if (!line)
			return (1);
		if (g_exit_status == 130)
		{
			free(line);
			return (-1);
		}
		if (hd_is_end(line, delim))
		{
			free(line);
			break;
		}
		hd_write(fd, line);
		free(line);
	}
	return (0);
}

// Gestion heredoc complète (copié de votre ami)
static char	*handle_heredoc(char *delimiter)
{
	char				*tmp_filename;
	int					tmp_fd;
	struct sigaction	old_int;
	int					saved;
	int					ret;

	tmp_fd = create_tmp_heredoc_file(&tmp_filename);
	if (tmp_fd == -1)
		return (NULL);

	saved = g_exit_status;
	disable_echoctl();
	hd_set_signals(&old_int);
	g_exit_status = 0;
	
	ret = heredoc_loop(delimiter, tmp_fd);
	
	enable_echoctl();
	hd_restore_signals(&old_int);
	close(tmp_fd);
	
	if (ret == 1 && g_exit_status != 130)
	{
		printf("\nminishell: warning: here-document delimited by end-of-file ");
		printf("(wanted `%s')\n", delimiter);
	}
	if (g_exit_status != 130)
		g_exit_status = saved;
	
	if (ret == -1 || g_exit_status == 130)
	{
		unlink(tmp_filename);
		free(tmp_filename);
		return (NULL);
	}
	
	return (tmp_filename);
}

// Retirer les arguments de redirection de la liste
static void	remove_redirect_args(char **args, int i)
{
	int	j;

	j = i;
	while (args[j + 2])
	{
		free(args[j]);
		args[j] = args[j + 2];
		j++;
	}
	free(args[j]);
	free(args[j + 1]);
	args[j] = NULL;
	args[j + 1] = NULL;
}

// Gérer un heredoc
static int	handle_heredoc_redirect(t_simple_data *data, int i, char **heredoc_file, int *saved_stdin)
{
	int	heredoc_fd;

	if (!data->args[i + 1])
		return (0);
	*heredoc_file = handle_heredoc(data->args[i + 1]);
	if (!*heredoc_file)
	{
		g_exit_status = 130;
		return (0);
	}
	heredoc_fd = open(*heredoc_file, O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("open heredoc");
		unlink(*heredoc_file);
		free(*heredoc_file);
		g_exit_status = 1;
		return (0);
	}
	*saved_stdin = dup(STDIN_FILENO);
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
	remove_redirect_args(data->args, i);
	return (1);
}

// Chercher un pipe dans les arguments
static int	find_pipe(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			return (i);
		i++;
	}
	return (-1);
}

// Exécuter une commande simple (sans pipe)
static void	execute_single_command(char **args, t_env *env)
{
	if (is_builtin(args[0]))
		handle_builtin(args, &env);
	else
		launch_extern_command_simple(args, env);
}

// Traiter les redirections
static int	process_redirections(t_simple_data *data, char **heredoc_file, int *saved_stdin)
{
	int	i;

	i = 0;
	while (data->args[i])
	{
		if (ft_strcmp(data->args[i], "<<") == 0)
		{
			if (!handle_heredoc_redirect(data, i, heredoc_file, saved_stdin))
				return (0);
			break;
		}
		else if (ft_strcmp(data->args[i], "<") == 0 || 
				 ft_strcmp(data->args[i], ">") == 0 || 
				 ft_strcmp(data->args[i], ">>") == 0)
		{
			printf("minishell: redirection not fully implemented yet\n");
			g_exit_status = 1;
			return (0);
		}
		i++;
	}
	return (1);
}
{
	int		pipefd[2];
	pid_t	pid1, pid2;
	int		status;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		g_exit_status = 1;
		return;
	}
	
	pid1 = fork();
	if (pid1 == 0)
	{
		// Première commande - écrit dans le pipe
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		reset_signals_to_default();
		if (is_builtin(left_cmd[0]))
		{
			handle_builtin(left_cmd, &env);
			exit(g_exit_status);
		}
		else
		{
			char **env_array = env_to_array(env);
			char *path = find_command_path(left_cmd[0], env);
			if (path && execve(path, left_cmd, env_array) == -1)
				exit(127);
			exit(127);
		}
	}
	
	pid2 = fork();
	if (pid2 == 0)
	{
		// Deuxième commande - lit depuis le pipe
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		reset_signals_to_default();
		if (is_builtin(right_cmd[0]))
		{
			handle_builtin(right_cmd, &env);
			exit(g_exit_status);
		}
		else
		{
			char **env_array = env_to_array(env);
			char *path = find_command_path(right_cmd[0], env);
			if (path && execve(path, right_cmd, env_array) == -1)
				exit(127);
			exit(127);
		}
	}
	
	// Parent - fermer les pipes et attendre
	close(pipefd[0]);
	close(pipefd[1]);
	ignore_sigint();
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	init_signals_prompt();
}
{
	int	i;

	i = 0;
	while (data->args[i])
	{
		if (ft_strcmp(data->args[i], "<<") == 0)
		{
			if (!handle_heredoc_redirect(data, i, heredoc_file, saved_stdin))
				return (0);
			break;
		}
		else if (ft_strcmp(data->args[i], "<") == 0 || 
				 ft_strcmp(data->args[i], ">") == 0 || 
				 ft_strcmp(data->args[i], ">>") == 0)
		{
			printf("minishell: redirection not fully implemented yet\n");
			g_exit_status = 1;
			return (0);
		}
		i++;
	}
	return (1);
}

// Nettoyer après exécution
static void	cleanup_execution(char *heredoc_file, int saved_stdin)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (heredoc_file)
	{
		unlink(heredoc_file);
		free(heredoc_file);
	}
}

// Exécution simple avec gestion des redirections de base
static void	simple_execute(t_simple_data *data)
{
	char	*heredoc_file;
	int		saved_stdin;
	
	heredoc_file = NULL;
	saved_stdin = -1;
	if (!data->args || !data->args[0])
		return;
	if (!process_redirections(data, &heredoc_file, &saved_stdin))
		return;
	if (is_builtin(data->args[0]))
		handle_builtin(data->args, &data->env);
	else
		launch_extern_command_simple(data->args, data->env);
	cleanup_execution(heredoc_file, saved_stdin);
}

// Processus enfant pour l'exécution
static void	child_process(char *path, char **args, t_env *env)
{
	char	**env_array;
	
	reset_signals_to_default();
	env_array = env_to_array(env);
	if (execve(path, args, env_array) == -1)
	{
		perror("execve");
		free_args(env_array);
		free(path);
		exit(127);
	}
}

// Processus parent pour l'attente
static void	parent_process(pid_t pid, char *path, int *status)
{
	ignore_sigint();
	waitpid(pid, status, 0);
	free(path);
	
	if (WIFEXITED(*status))
		g_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		g_exit_status = 128 + WTERMSIG(*status);
	
	handle_status_and_print(*status);
	init_signals_prompt();
}

// Fonction d'exécution externe simplifiée (basée sur votre ami)
void	launch_extern_command_simple(char **args, t_env *env)
{
	pid_t	pid;
	char	*path;
	int		status;

	if (!args || !args[0])
	{
		g_exit_status = 127;
		return;
	}
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = find_command_path(args[0], env);
	if (!path)
	{
		print_error(args[0], "command not found\n");
		g_exit_status = 127;
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(path);
		g_exit_status = 1;
		return;
	}
	if (pid == 0)
		child_process(path, args, env);
	else
		parent_process(pid, path, &status);
}

// Traiter la ligne de commande
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

// Boucle principale simplifiée (exactement comme votre ami)
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
