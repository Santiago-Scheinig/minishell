/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:54:48 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:54:31 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Copies 'n' bytes from source to destination memory.
 *
 * 			Copies 'n' bytes from the memory area 'src' to 'dest'. If both
 *			'src' and 'dest' are NULL, does nothing. Internally uses
 *			ft_memmove to handle the copy.
 *
 * @param	dest	Pointer to the destination memory area.
 * @param	src		Pointer to the source memory area.
 * @param	n		Number of bytes to copy.
 *
 * @return	Pointer to 'dest'.
 */
void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	if (!src && !dest)
		return (dest);
	ft_memmove(dest, src, n);
	return (dest);
}
