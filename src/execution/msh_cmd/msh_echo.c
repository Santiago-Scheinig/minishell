/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/25 21:41:54 by sscheini         ###   ########.fr       */
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
void	msh_echo(char **args)
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
		if (write(1, args[i], ft_strlen(args[i])) == -1)
		{
			return (perror("msh: write error"));
		}
		if (args[i + 1])
			if (write(1, " ", 1) == -1)
				return (perror("msh: write error"));
		i++;
	}
	if (new_line)
		if (write(1, "\n", 1) == -1)
			return (perror("msh: write error"));
}
