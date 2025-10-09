/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 05:56:34 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

static int	env_print(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (write(STDOUT_FILENO, envp[i], ft_strlen(envp[i])) == -1)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (MSHELL_FAILURE);
		i++;
	}
	return (MSHELL_SUCCESS);
}

/**
 * Builterr
 */
int	msh_env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	else if (args[1])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	if (env_print(envp))
		return (built_end(args[0], "System failed", NULL, '\0'));
	return (MSHELL_SUCCESS);
}
