/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 18:55:11 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

int	msh_pwd(char **args, t_list *envp)
{
	char	*path;

	path = NULL;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	path = getcwd(NULL, 0);
	if (!path)
	{
		path = ft_strdup(shell_getenv(envp, "PWD"));
		write(1, path, ft_strlen(path));
	}
	else
	{
		write(1, path, ft_strlen(path));
		write(1, "\n", 1);
	}
	free(path);
	return (0);
}
