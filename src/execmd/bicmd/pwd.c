/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/07 16:18:42 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

/**
 * GORGEOUS!
 * But dont forget! the ERRORS must be printed in CHILDEND after the waitpid, in pipex
 * or the return of this built-in, thats how we properly follow up errors, we also
 * need to google which error is which or read it in the manual.
 */
int	pwd(char **args)
{
	char	*path;

	path = NULL;
	if (args[1] && args[1][0] == '-')
	{
		built_end(args[0], "Invalid flags", "[]", args[1][1]);
		return (0);
	}
	path = getcwd(NULL, 0);
	if (!path)
	{
		built_end("pwd", "System failed", NULL, '\0');
		return (0);
	}
	ft_printf("%s\n", path);
	free(path);
	return (1);
}
