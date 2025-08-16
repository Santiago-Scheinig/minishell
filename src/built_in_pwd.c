/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/16 19:01:16 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	built_pwd(char **args)
{
	char	*path;

	path = NULL;
	if (args[1][0] == '-')
	{
		ft_printf("pwd: %c: invalid option\n", args[1][1]);
		ft_printf("pwd: usage: pwd []");
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
