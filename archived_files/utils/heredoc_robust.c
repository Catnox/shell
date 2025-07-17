/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_robust.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Structure pour gérer les fichiers temporaires de heredoc
typedef struct s_heredoc_manager
{
	char	**temp_files;
	int		count;
	int		capacity;
}	t_heredoc_manager;

static t_heredoc_manager g_heredoc_manager = {NULL, 0, 0};

// Génère un nom de fichier temporaire unique
static char	*generate_temp_filename(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*filename;
	
	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	
	filename = ft_strjoin("/tmp/minishell_heredoc_", num_str);
	free(num_str);
	
	return (filename);
}

// Ajoute un fichier temporaire à la liste pour le cleanup
static int	add_temp_file(char *filename)
{
	char	**new_files;
	int		new_capacity;
	
	if (g_heredoc_manager.count >= g_heredoc_manager.capacity)
	{
		new_capacity = g_heredoc_manager.capacity == 0 ? 10 : g_heredoc_manager.capacity * 2;
		new_files = ft_realloc(g_heredoc_manager.temp_files, 
			sizeof(char *) * g_heredoc_manager.capacity,
			sizeof(char *) * new_capacity);
		if (!new_files)
			return (0);
		
		g_heredoc_manager.temp_files = new_files;
		g_heredoc_manager.capacity = new_capacity;
	}
	
	g_heredoc_manager.temp_files[g_heredoc_manager.count] = ft_strdup(filename);
	if (!g_heredoc_manager.temp_files[g_heredoc_manager.count])
		return (0);
	
	g_heredoc_manager.count++;
	return (1);
}

// Crée un fichier temporaire pour heredoc
static char	*create_heredoc_temp_file(void)
{
	char	*filename;
	int		fd;
	
	filename = generate_temp_filename();
	if (!filename)
		return (NULL);
	
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filename);
		return (NULL);
	}
	close(fd);
	
	if (!add_temp_file(filename))
	{
		unlink(filename);
		free(filename);
		return (NULL);
	}
	
	return (filename);
}

// Expansion sécurisée des variables dans heredoc
static char	*expand_heredoc_line_safe(char *line, t_env *env, int should_expand)
{
	char	*expanded;
	t_shell	dummy_shell;
	
	if (!should_expand || !line)
		return (ft_strdup(line));
	
	// Crée une structure shell temporaire pour expand_variables
	dummy_shell.env_list = env;
	expanded = expand_variables(line, &dummy_shell);
	return (expanded ? expanded : ft_strdup(line));
}

// Vérifie si le délimiteur contient des quotes (pas d'expansion)
static int	delimiter_has_quotes(char *delimiter)
{
	int	i;
	
	if (!delimiter)
		return (0);
	
	i = 0;
	while (delimiter[i])
	{
		if (delimiter[i] == '\'' || delimiter[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

// Écrit le contenu du heredoc dans le fichier
static int	write_heredoc_content_safe(int fd, char *delimiter, t_env *env)
{
	char	*line;
	char	*expanded_line;
	int		should_expand;
	int		bytes_written;
	
	should_expand = !delimiter_has_quotes(delimiter);
	
	while (1)
	{
		line = readline("heredoc> ");
		
		// Si readline retourne NULL, c'est EOF ou interruption
		if (!line)
		{
			// C'est EOF (Ctrl+D)
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(delimiter, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			break;
		}
		
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		
		expanded_line = expand_heredoc_line_safe(line, env, should_expand);
		free(line);
		
		if (!expanded_line)
			return (0);
		
		bytes_written = write(fd, expanded_line, ft_strlen(expanded_line));
		if (bytes_written < 0)
		{
			free(expanded_line);
			return (0);
		}
		
		bytes_written = write(fd, "\n", 1);
		free(expanded_line);
		
		if (bytes_written < 0)
			return (0);
	}
	
	return (1);
}

// Fonction principale pour gérer le heredoc
char	*handle_heredoc_robust(char *delimiter, t_env *env)
{
	char	*temp_file;
	int		fd;
	int		success;
	
	if (!delimiter)
		return (NULL);
	
	temp_file = create_heredoc_temp_file();
	if (!temp_file)
		return (NULL);
	
	fd = open(temp_file, O_WRONLY | O_APPEND);
	if (fd < 0)
	{
		free(temp_file);
		return (NULL);
	}
	
	// Configurer les signaux pour heredoc (comme votre ami)
	struct sigaction old_int;
	hd_set_signals(&old_int);
	
	success = write_heredoc_content_safe(fd, delimiter, env);
	close(fd);
	
	// Restaurer les signaux interactifs
	hd_restore_signals(&old_int);
	init_signals_prompt();
	
	if (!success)
	{
		unlink(temp_file);
		free(temp_file);
		return (NULL);
	}
	
	return (temp_file);
}

// Nettoie tous les fichiers temporaires de heredoc
void	cleanup_heredoc_files(void)
{
	int	i;
	
	if (!g_heredoc_manager.temp_files)
		return;
	
	i = 0;
	while (i < g_heredoc_manager.count)
	{
		if (g_heredoc_manager.temp_files[i])
		{
			unlink(g_heredoc_manager.temp_files[i]);
			free(g_heredoc_manager.temp_files[i]);
		}
		i++;
	}
	
	free(g_heredoc_manager.temp_files);
	g_heredoc_manager.temp_files = NULL;
	g_heredoc_manager.count = 0;
	g_heredoc_manager.capacity = 0;
}

// Ouvre un fichier heredoc pour lecture
int	open_heredoc_file(char *filename)
{
	int	fd;
	
	if (!filename)
		return (-1);
	
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		perror("minishell: heredoc file");
	
	return (fd);
}

// Traite les heredocs dans une commande simple
int	process_heredocs_simple(t_simple_cmd *cmd, t_env *env)
{
	t_redir	*redir;
	char	*temp_file;
	
	if (!cmd || !cmd->redirs)
		return (0);
		
	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			temp_file = handle_heredoc_robust(redir->file, env);
			if (!temp_file)
				return (-1);
				
			// Remplacer le délimiteur par le nom du fichier temporaire
			free(redir->file);
			redir->file = temp_file;
			
			// Changer le type vers redirection d'entrée normale
			redir->type = REDIR_IN;
		}
		redir = redir->next;
	}
	
	return (0);
}
