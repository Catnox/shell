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

int	main(int argc, char *argv[], char *env[])
{
	t_env	*env_list;

	(void)argc, (void)argv;
	env_list = NULL;
	env_list = init_env(&env_list, env);
	if (!env_list)
	{
		ft_putstr_fd("minishell: failed to initialize environment\n", 2);
		return (1);
	}
	simple_minishell_loop(env_list);
	free_env(env_list);
	return (g_exit_status);
}
