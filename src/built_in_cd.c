/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/17 13:31:51 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	built_cd(t_body *minishell, char **args)
{
	char	*dir;

	dir = NULL;
	if (args[1])
	{
		perror("cd : too much args");
		return (0);
	}
	if (!args[0])
	{
		dir = getenv("HOME");
		if (!dir)
		{
			perror("cd: HOME not set");
			return (0);
		}
	}
	else
		dir = args[0];
	if (chdir(dir) != 0)
	{
		ft_printf("cd : %s: No such file or directory", dir);
		return (0);
	}
	if (!path_minishell(minishell))
		return (0);
	return (1);
}
