/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/21 20:27:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Implements the 'exit' built-in command.
 *
 *			Terminates the shell process, performing a clean shutdown.
 *			If an argument is provided, it is interpreted as the exit
 *			status code. The command may print "exit" when running in
 *			interactive mode.
 *
 * @param	args	Array of arguments. May include one optional numeric
 *					exit code (e.g., "exit 42"). Additional arguments
 *					trigger an error.
 * @param	msh		Pointer to the main shell structure (t_body) used for
 *					cleanup and interactive handling.
 *
 * @note	If running interactively, writes "exit" to STDOUT before
 *			terminating.
 * @note	If more than one argument is passed, returns an error via
 *			shell_builterr() without exiting immediately.
 * @note	If the argument after 'exit' is non-numeric, reports an
 *			INVARGV error and exits with that error code.
 * @note	If no arguments are given, exits with MSHELL_SUCCESS (0).
 *
 * @return	Always returns MSHELL_FAILURE if reached, though the function
 *			normally terminates the process via exit().
 */
int	bin_exit(char **args, t_body *msh)
{
	int	i;

	i = 0;
	if (msh && msh->interactive)
		if (write(STDOUT_FILENO, "exit\n", 5) == -1)
			return (shell_binerr(SYSFAIL, "exit", "write", 0));
	if ((args && args[1] && args[2]))
		return (shell_binerr(INVARGC, "exit", NULL, '\0'));
	if (args && args[1])
	{
		if (msh)
			shell_cleanup(true, msh);
		while (ft_isdigit(args[1][i]))
			i++;
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		exit(shell_binerr(INVARGV, "exit", args[1], 0));
	}
	if (msh)
		shell_cleanup(true, msh);
	exit(MSHELL_SUCCESS);
	return (MSHELL_FAILURE);
}
