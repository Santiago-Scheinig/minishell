/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 15:55:21 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

int	built_pwd(char **args)
{
	char	*path;

	path = NULL;
	if (args[1][0] == '-')
	{
		built_end(args[0], "Invalid flags", "[]", args[1][1]);
		return (0);
	}
	path = getcwd(NULL, 0);
	if (!path)
	{
		perror("pwd");
		return (0);
	}
	ft_printf("%s\n", path);
	free(path);
	return (1);
}
