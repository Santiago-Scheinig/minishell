/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/16 14:06:31 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bulitin.h"

void	built_echo(char **args)
{
	int	new_line;
	int	i;

	i = 1;
	new_line = 1;
	if (args[1] && !ft_strncmp(args[1], "-n", 3))
	{
		i++;
		new_line = 0;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i++ + 1])
			ft_printf(" ");
	}
	if (new_line)
		ft_printf("\n");
}

int	built_cd(char **args)
{
	if (!args[1])
	{
		perror("incorrect path");
		return (0);
	}
	if (chdir(args[1]) != 0)
	{
		perror("cd");
		return (0);
	}
	if (!path_minishell(minishell))
		return (0);
	return (1);
}
