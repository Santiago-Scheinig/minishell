/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/16 13:49:08 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"
#include "troubleshoot.h"

void	end_minishell(t_body *minishell)
{
	shell_lstclear(&minishell->envp_lst, shell_lstdelvar);
	ft_split_free(minishell->envp);
	if (minishell->interactive)
		rl_clear_history();
	forcend(minishell, NULL, 0);
}

int	b_exit(char **args, t_body *minishell)
{
	int	i;

	i = 0;
	ft_printf("exit\n");
	if (args[1] && args[2])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	end_minishell(minishell);
	if (args[1])
	{
		while (ft_isdigit(args[1][i]))
			i++;
		if (!args[1][i])
			exit(ft_atoi(args[1]));
		built_end(args[0], "Numeric arg required", args[1], '\0');
		exit(2);
	}
	exit(0);
	return (0);
}
