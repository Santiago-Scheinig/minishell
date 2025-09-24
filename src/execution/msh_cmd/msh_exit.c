/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 20:13:04 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

/**
 * Exits the shell process with an optional numeric status.
 * 
 * @param args Array of arguments passed to exit (args[0] is "exit").
 * @param minishell Pointer to the shell context; used for interactive behavior and cleanup.
 * 
 * If running interactively prints "exit". Validates argument count and numeric argument.
 * 
 * @return void (function exits the process on success or error cases).
 * @note - If more than one numeric argument is provided, prints error and does not exit.
 * - Performs cleanup/forced end via forcend() when needed.
 */
void	msh_exit(char **args, t_body *minishell)
{
	int	i;

	i = 0;
	if (minishell->interactive)
		write(1, "exit\n", 5);
	if ((args && args[1] && args[2]))
	{
		built_end(args[0], "Numbers of args", NULL, '\0');
		return ;
	}
	if (args && args[1])
	{
		while (ft_isdigit(args[1][i]))
		i++;
		if (minishell)
			forcend(minishell, "malloc", MSHELL_SUCCESS);
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		built_end(args[0], "Numeric arg required", args[1], '\0');
		exit(2);
	}
	exit(0);
}
