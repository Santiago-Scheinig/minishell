/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/14 14:09:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Implements the 'echo' built-in command.
 *
 *			Prints the provided arguments to standard output, separated
 *			by spaces. By default, a trailing newline is printed unless
 *			the '-n' flag is used as the first argument.
 *
 * @param	args	Array of argument strings. The first element is "echo",
 *					and subsequent elements are strings to be printed.
 *
 * @note	Supports the '-n' flag to suppress the trailing newline.
 * @note	If any write() call fails, reports an error through
 *			shell_binerr() and returns the corresponding system code.
 * @note	Each argument is written to STDOUT and separated by a space
 *			unless it is the last one.
 *
 * @return	MSHELL_SUCCESS (0) on success, or system error code if a
 *			write() operation fails.
 */
int	bin_echo(char **args)
{
	int	new_line;
	int	i;

	i = 1;
	new_line = 1;
	while (args[i] && !ft_strncmp(args[i], "-n", 3))
	{
		i++;
		new_line = 0;
	}
	while (args[i])
	{
		if (write(STDOUT_FILENO, args[i], ft_strlen(args[i])) == -1)
			return (shell_binerr(SYSFAIL, "echo", "write", '\0'));
		if (args[i + 1])
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (shell_binerr(SYSFAIL, "echo", "write", '\0'));
		i++;
	}
	if (new_line)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (shell_binerr(SYSFAIL, "echo", "write", '\0'));
	return (MSHELL_SUCCESS);
}
