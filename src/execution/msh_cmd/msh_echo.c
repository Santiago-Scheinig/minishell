/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 15:51:39 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

/**
 * Prints arguments to standard output, separated by spaces.
 * 
 * @param args Array of arguments passed to the echo command
 * (args[0] is "echo").
 * 
 * @note - If any leading argument(s) are "-n", no trailing newline is printed.
 */
int	msh_echo(char **args)
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
			return (built_end(args[0], "System failed", NULL, '\0'));
		if (args[i + 1])
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (built_end(args[0], "System failed", NULL, '\0'));
		i++;
	}
	if (new_line)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (built_end(args[0], "System failed", NULL, '\0'));
	return (MSHELL_SUCCESS);
}
