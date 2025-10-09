/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 05:58:35 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * Builterr
 */
int	msh_exit(char **args, t_body *msh)
{
	int	i;

	i = 0;
	if (msh && msh->interactive)
		if (write(STDOUT_FILENO, "exit\n", 5) == -1)
			return (MSHELL_FAILURE);
	if ((args && args[1] && args[2]))
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	if (msh)
		shell_cleanup(msh);
	if (args && args[1])
	{
		while (ft_isdigit(args[1][i]))
			i++;
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		exit(built_end(args[0], "Numeric arg required", args[1], '\0'));
	}
	exit(MSHELL_SUCCESS);
	return (MSHELL_FAILURE);
}
