/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:45:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

/**
 * No minishell structure, no error printing! 
 * CD has to return the errno 
 * equivalent to that specific error. Then the CHILDEND inside of pipex prints
 * the error. I Don't know the error number, we have to search for it in google
 * or the manual.
 */
int	msh_cd(char **args, t_list *envp)
{
	char	*dir;

	dir = NULL;
	if (!args[1])
	{
		dir = shell_getenv(envp, "HOME");
		if (!dir)
			return (built_end(args[0], "HOME", NULL, '\0'));
	}
	else if (args[1] && args[1][0] == '-')
	{
		return (built_end(args[0], "Invalid flags", "[dir]", args[1][1]));
	}
	else if (!args[2])
		dir = args[1];
	else
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	if (chdir(dir) != 0)
		return (built_end(args[0], "System failed", NULL, '\0'));
	return (0);
}
