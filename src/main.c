/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

// Main simplifié pour architecture simple
int	main(int argc, char *argv[], char *env[])
{
	t_env	*env_list;

	(void)argc, (void)argv;
	
	// Initialiser l'environnement
	env_list = NULL;
	env_list = init_env(&env_list, env);
	
	if (!env_list)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	
	// Utiliser la boucle simplifiée comme votre ami
	simple_minishell_loop(env_list);
	
	// Nettoyage
	free_env(env_list);
	return (g_exit_status);
}
