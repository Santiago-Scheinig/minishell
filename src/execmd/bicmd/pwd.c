/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/25 22:25:17 by sscheini         ###   ########.fr       */
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
	if (args[1][0] == '-')
	{
		ft_printf("pwd: %c: invalid option\n", args[1][1]);
		ft_printf("pwd: usage: pwd []");
		return (0);
	}
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (0);
	}
	ft_printf("%s\n", path);
	free(path);
	return (1);
}
