/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:02:12 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:55:22 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Fills a memory area with a constant byte.
 *
 * 			Sets the first 'n' bytes of the memory area pointed to by 's'
 *			to the byte value 'c'.
 *
 * @param	s	Pointer to the memory area to fill.
 * @param	c	Byte value to set (converted to unsigned char).
 * @param	n	Number of bytes to set.
 *
 * @return	Pointer to the memory area 's'.
 */
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*tmp;
	size_t			i;

	i = 0;
	tmp = (unsigned char *) s;
	while (i < n)
		tmp[i++] = (unsigned char) c;
	return (s);
}
