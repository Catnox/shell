/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_simple.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_exit_status;

// Handler pour SIGINT en mode prompt (exactement comme votre ami)
static void	sigint_prompt(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

// Handler pour SIGINT pendant heredoc (exactement comme votre ami)
static void	sigint_hd(int sig)
{
	(void)sig;
	g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
}

// Initialiser les signaux pour le prompt (copié de votre ami)
void	init_signals_prompt(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_prompt;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

// Gérer le status et l'affichage (exactement comme votre ami)
void	handle_status_and_print(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT || sig == SIGQUIT)
		{
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (sig == SIGQUIT)
				write(STDOUT_FILENO, "Quit\n", 5);
			g_exit_status = 128 + sig;
		}
	}
	else
		g_exit_status = WEXITSTATUS(status);
}

// Remettre les signaux par défaut (copié de votre ami)
void	reset_signals_to_default(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

// Ignorer SIGINT (copié de votre ami)
void	ignore_sigint(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
}

// Configurer les signaux pour heredoc (copié de votre ami)
void	hd_set_signals(struct sigaction *old_int)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_hd;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, old_int);
	signal(SIGQUIT, SIG_IGN);
}

// Restaurer les signaux après heredoc (copié de votre ami)
void	hd_restore_signals(const struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
}

// Désactiver ECHOCTL (copié de votre ami)
void	disable_echoctl(void)
{
	struct termios	t;

	if (tcgetattr(STDIN_FILENO, &t) == -1)
		return ;
	t.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

// Réactiver ECHOCTL (copié de votre ami)
void	enable_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
