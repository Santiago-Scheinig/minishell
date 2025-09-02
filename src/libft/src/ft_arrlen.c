/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:37 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/02 21:03:58 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * COMMENT PENDING
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
