/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 09:18:16 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * Builterr
 */
int	bin_exit(char **args, t_body *msh)
{
	int	i;

	i = 0;
	if (msh && msh->interactive)
		if (write(STDOUT_FILENO, "exit\n", 5) == -1)
			return (shell_builterr(SYSFAIL, "exit", NULL, 0));
	if ((args && args[1] && args[2]))
		return (shell_builterr(INVARGC, "exit", NULL, '\0'));
	if (msh)
		shell_cleanup(true, msh);
	if (args && args[1])
	{
		while (ft_isdigit(args[1][i]))
			i++;
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		exit(shell_builterr(INVARGV, "exit", args[1], 0));
	}
	exit(MSHELL_SUCCESS);
	return (MSHELL_FAILURE);
}
