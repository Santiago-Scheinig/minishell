/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:02:06 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/25 22:21:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

/**
 * GORGEOUS!
 */
void	echo(char **args)
{
	int	new_line;
	int	i;

	i = 1;
	new_line = 1;
	if (args[0] && !ft_strncmp(args[0], "-n", 3))
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
