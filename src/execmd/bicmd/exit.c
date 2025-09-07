/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 14:51:38 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/07 17:55:03 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

int	b_exit(char **args)
{
	int	i;

	i = 0;
	ft_printf("exit\n");
	if (args[1] && args[2])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	else if (args[1])
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
