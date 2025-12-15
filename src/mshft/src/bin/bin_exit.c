/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/12/15 11:32:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

int	get_exit_no(char *arg_no)
{
	int	i;
	int	exit_no;

	exit_no = MSHELL_SUCCESS;
	if (!ft_isdigit(arg_no[0]) && arg_no[0] != '+' && arg_no[0] != '-')
	{
		shell_binerr(INVARGV, "exit", arg_no, 0);
		return (exit_no);
	}
	i = 1;
	while (ft_isdigit(arg_no[i]))
		i++;
	if (!arg_no[i])
		return (ft_atoi(arg_no));
	shell_binerr(INVARGV, "exit", arg_no, 0);
	return (exit_no);
}

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
	int			i;
	int			exit_no;

	i = 0;
	if (msh && msh->interactive)
		if (write(STDOUT_FILENO, "exit\n", 5) == -1)
			return (shell_binerr(SYSFAIL, "exit", "write", 0));
	if ((args && args[1] && args[2]))
		return (shell_binerr(INVARGC, "exit", NULL, '\0'));
	if (args && args[1])
	{
		exit_no = get_exit_no(args[1]);
		if (msh)
			shell_cleanup(true, msh);
		if (exit_no)
			exit(exit_no);
		exit(MSHELL_FAILURE);
	}
	if (msh)
		shell_cleanup(true, msh);
	exit(MSHELL_SUCCESS);
	return (MSHELL_FAILURE);
}
