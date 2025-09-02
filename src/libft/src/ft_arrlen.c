/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:37 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/31 18:42:58 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
