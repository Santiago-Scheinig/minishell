/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 15:54:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

void	msh_exit(char **args, t_body *minishell)
{
	int	i;

	i = 0;
	if (minishell->interactive)
		write(1, "exit\n", 5);
	if ((args && args[1] && args[2]))
	{
		built_end(args[0], "Numbers of args", NULL, '\0');
		return ;
	}
	if (args && args[1])
	{
		while (ft_isdigit(args[1][i]))
			i++;
		if (minishell)
			forcend(minishell, "malloc", MSHELL_SUCCESS);
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		built_end(args[0], "Numeric arg required", args[1], '\0');
		exit(2);
	}
	exit(0);
}
