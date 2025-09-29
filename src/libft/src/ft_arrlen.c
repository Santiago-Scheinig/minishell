/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:37 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/27 14:44:13 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Counts the amount of byts on a CONST VOID.
 * 
 * @param arr The CONST VOID to count bytes from.
 * @return An INT with the amount of bytes counted.
 * @note This function doesn't count the NULL.
 */
int	ft_arrlen(const void **arr)
{
	int			i;

	i = 0;
	if (!arr || !arr[i])
		return (i);
	while (arr[i])
		i++;
	return (i);
}
