/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 00:00:00 by radubos           #+#    #+#             */
/*   Updated: 2025/07/17 00:00:00 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_exit(char **args, t_shell *shell)
{
	int	exit_code;

	printf("exit\n");
	
	exit_code = g_exit_status;
	if (args[1])
	{
		if (ft_isnumeric(args[1]))
			exit_code = ft_atoi(args[1]);
		else
		{
			print_error("exit", "numeric argument required");
			exit_code = 2;
		}
	}
	
	(void)shell; // Not used in new version
	exit(exit_code);
}
