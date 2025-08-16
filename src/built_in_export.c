/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/16 19:10:57 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"


static void	print_envp(t_body *minishell)
{
	
}

t_env	*built_envps(t_body *minishell)
{
	
}

t_env	*built_export(t_body *minishell, char **args, char **envp)
{
	int	i;

	i = 0;
	if (!args[1])
	{
		while (envp[i])
		{
			if (minishell->env[i] && minishell->env->export[i])
				ft_printf("%s=%s\n", minishell->env[i], minishell->env[i]);
			ft_printf("%s\n", envp[i++]);
		}
		return (1);
	}
	i = 1;
	while ()
	{
	}
	return (minishell);
}