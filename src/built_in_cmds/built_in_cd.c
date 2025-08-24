/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 17:57:16 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	built_cd(t_body *minishell, char **args)
{
	char	*dir;

	dir = NULL;
	if (!args[1])
	{
		dir = getenv("HOME");
		if (!dir)
			built_end(args[0], "System failed", NULL, '\0');
	}
	else if (args[1] && args[1][0] == '-')
	{
		if (args[1][1])
			built_end(args[0], "Invalid flags", "[dir]", args[1][1]);
		built_end(args[0], "Invalid flags", "[dir]", '\0');
	}
	else if (!args[2])
		dir = args[1];
	else
		built_end(args[0], "Numbers of args", NULL, '\0');
	if (chdir(dir) != 0)
		built_end(args[0], "System failed", NULL, '\0');
	if (!shell_prompt(minishell))
		return (0);
	return (1);
}
