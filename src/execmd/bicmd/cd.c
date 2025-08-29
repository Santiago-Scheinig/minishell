/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/29 17:35:13 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * No minishell structure, no error printing! 
 * CD has to return the errno 
 * equivalent to that specific error. Then the CHILDEND inside of pipex prints
 * the error. I Don't know the error number, we have to search for it in google
 * or the manual.
 */
int	cd(char **args, char **envp)
{
	char	*dir;

	dir = NULL;
	if (!args[1])
	{
		dir = shell_getenv(envp, "HOME");
		if (!dir)
			built_end(args[0], "System failed", NULL, '\0');
	}
	else if (args[1] && args[1][0] == '-')
	{
		if (args[1][1])
			built_end(args[0], "Invalid flags", "[dir]", args[1][1]);
		built_end(args[0], "Invalid flags", "[dir]", '\0');
	}
	else if (!args[2])
		dir = args[1];
	else
		built_end(args[0], "Numbers of args", NULL, '\0');
	if (chdir(dir) != 0)
		built_end(args[0], "System failed", NULL, '\0');
	return (1);
}
