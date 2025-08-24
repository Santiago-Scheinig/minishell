/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 13:51:35 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

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
