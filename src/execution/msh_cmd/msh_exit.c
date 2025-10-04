/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 22:20:40 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

static void	clear_msh(t_body *minishell)
{
	if (minishell->interactive)
		rl_clear_history();
	shell_lstclear(&(minishell->lst_t_var), shell_lstdel_var);
	ft_split_free(minishell->envp);
	cleanup(minishell);
}

/**
 * Exits the shell process with an optional numeric status.
 * 
 * @param args Array of arguments passed to exit (args[0] is "exit").
 * @param minishell Pointer to the shell context; used for interactive 
 * behavior and cleanup.
 * 
 * If running interactively prints "exit". Validates argument count and 
 * numeric argument.
 * 
 * @return void (function exits the process on success or error cases).
 * @note - If more than one numeric argument is provided, prints error 
 * and does not exit.
 * - Performs cleanup/forced end via forcend() when needed.
 */
int	msh_exit(char **args, t_body *minishell)
{
	int	i;

	i = 0;
	if (minishell && minishell->interactive)
		if (write(1, "exit\n", 5) == -1)
			return (MSHELL_FAILURE);
	if ((args && args[1] && args[2]))
	{
		built_end(args[0], "Numbers of args", NULL, '\0');
		return (MSHELL_FAILURE);
	}
	if (minishell)
		clear_msh(minishell);
	if (args && args[1])
	{
		while (ft_isdigit(args[1][i]))
			i++;
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		built_end(args[0], "Numeric arg required", args[1], '\0');
		exit(MSHELL_MISSUSE);
	}
	exit(MSHELL_SUCCESS);
	return (MSHELL_FAILURE);
}
