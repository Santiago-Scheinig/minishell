/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/23 15:09:02 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

void	msh_exit(char **args, t_body *minishell)
{
	int	i;

	i = 0;
	if (minishell->interactive)
	{
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
			if (!args[1][i])
				forcend(minishell, NULL, ft_atoi(args[1]));
			built_end(args[0], "Numeric arg required", args[1], '\0');
			forcend(minishell, NULL, 2);
		}
	}
	forcend(minishell, NULL, 0);
}
