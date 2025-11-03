/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/18 20:31:22 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Prints all environment variables to standard output.
 *
 *			Iterates through the provided environment array and writes
 *			each entry followed by a newline. Stops when a NULL entry
 *			is encountered.
 *
 * @param	envp	Array of strings representing the environment. Each
 *					entry should be formatted as "NAME=VALUE".
 *
 * @note	If any write() call fails, the function immediately returns
 *			MSHELL_FAILURE without printing remaining entries.
 *
 * @return	MSHELL_SUCCESS (0) on success, or MSHELL_FAILURE on a write
 *			error.
 */
static int	env_print(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (write(STDOUT_FILENO, envp[i], ft_strlen(envp[i])) == -1)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (MSHELL_FAILURE);
		i++;
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Implements the 'env' built-in command.
 *
 *			Prints all current environment variables. The command does
 *			not accept additional arguments or flags other than the
 *			command name itself.
 *
 *			Static helper:
 *				- env_print():	Iterates through envp and prints each
 *								variable line-by-line.
 *
 * @param	args	Array of command arguments. Only args[0] ("env") is
 *					expected; additional entries cause an error.
 * @param	envp	Current environment array of "NAME=VALUE" strings.
 *
 * @note	If an invalid flag or extra argument is provided, an error
 *			is reported using shell_builterr().
 * @note	Returns an error if a write() system call fails during
 *			environment printing.
 *
 * @return	MSHELL_SUCCESS (0) on success, or MSHELL_FAILURE / system
 *			error code on failure.
 */
int	bin_env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
		return (shell_binerr(INVFLGS, "env", "\0", args[1][1]));
	else if (args[1])
		return (shell_binerr(INVARGC, "env", "\0", '\0'));
	if (env_print(envp))
		return (shell_binerr(SYSFAIL, "env", "write", '\0'));
	return (MSHELL_SUCCESS);
}
