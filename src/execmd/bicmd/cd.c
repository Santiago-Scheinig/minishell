/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/25 22:21:19 by sscheini         ###   ########.fr       */
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
int	cd(t_body *minishell, char **args)
{
	char	*dir;

	dir = NULL;
	if (args[1])
	{
		perror("cd : too much args");
		return (0);
	}
	if (!args[0])
	{
		dir = getenv("HOME");
		if (!dir)
		{
			perror("cd: HOME not set");
			return (0);
		}
	}
	else
		dir = args[0];
	if (chdir(dir) != 0)
	{
		ft_printf("cd : %s: No such file or directory", dir);
		return (0);
	}
	if (!shell_prompt(minishell))
		return (0);
	return (1);
}
