/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 05:59:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * Builterr
 */
int	msh_pwd(char **args, t_list *lst_t_var)
{
	char	*path;

	path = NULL;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	path = getcwd(NULL, 0);
	if (!path)
	{
		path = ft_strdup(shell_getenv(lst_t_var, "PWD"));
		if (write(STDOUT_FILENO, path, ft_strlen(path)) == -1)
			return (built_end(args[0], "System failed", NULL, '\0'));

	}
	else
	{
		if (write(STDOUT_FILENO, path, ft_strlen(path)) == -1)
			return (built_end(args[0], "System failed", NULL, '\0'));
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (built_end(args[0], "System failed", NULL, '\0'));
	}
	free(path);
	return (0);
}
