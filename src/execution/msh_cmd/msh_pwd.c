/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:46:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

int	msh_pwd(char **args)
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
