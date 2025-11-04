/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 19:32:51 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:54:56 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Copies 'n' bytes from source to destination safely.
 *
 * 			Copies 'n' bytes from the memory area 'src' to 'dest'. Handles
 *			overlapping memory regions correctly to prevent data corruption.
 *
 * @param	dest	Pointer to the destination memory area.
 * @param	src		Pointer to the source memory area.
 * @param	n		Number of bytes to copy.
 *
 * @return	Pointer to 'dest'.
 */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dst_tmp;
	unsigned char	*src_tmp;
	size_t			i;

	i = 0;
	if (!dest && !src)
		return (dest);
	src_tmp = (unsigned char *) src;
	dst_tmp = (unsigned char *) dest;
	if (dst_tmp > src_tmp)
	{
		while (n > i)
		{
			n--;
			dst_tmp[n] = src_tmp[n];
		}
		return (dest);
	}
	while (i < n)
	{
		dst_tmp[i] = src_tmp[i];
		i++;
	}
	return (dest);
}
