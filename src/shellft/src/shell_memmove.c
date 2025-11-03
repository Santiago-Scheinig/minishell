/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_memmove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:58:54 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/18 17:57:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

static void	iq_moveup(char *dst_tmp, char *src_tmp, char *src_mask, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < n)
	{
		if (src_mask[i] == 'O')
		{
			i++;
			continue ;
		}
		if (i < n)
			dst_tmp[j] = src_tmp[i];
		i++;
		j++;
	}
}

static void	iq_movedn(char *dst_tmp, char *src_tmp, char *src_mask, size_t len)
{
	size_t	i;
	size_t	dst_len;

	i = -1;
	dst_len = len;
	while (src_tmp[++i] && len > i)
		if (src_mask[i] == 'O')
			dst_len--;
	while (len > 0 && dst_len > 0)
	{
		if (len > 0)
		{
			len--;
			if (len > 0 && src_mask[len] != 'O')
			{
				dst_len--;
				dst_tmp[dst_len] = src_tmp[len];
			}
		}
	}
	dst_len--;
	if (!dst_len && !len)
		dst_tmp[dst_len] = src_tmp[len];
}

/**
 * @brief	Moves memory from src to dest with a mask, safely handling overlap.
 *
 *			Performs a memory copy of n bytes from src to dest. If dest is
 *			higher in memory than src, calls iq_movedn() to copy backward.
 *			Otherwise, calls iq_moveup() to copy forward. Each byte is
 *			controlled by the corresponding entry in the mask array.
 *
 * @param	dest	Pointer to the destination memory buffer.
 * @param	src		Pointer to the source memory buffer.
 * @param	mask	Pointer to a mask array controlling which bytes are copied.
 * @param	n		Number of bytes to move.
 *
 * @note	If both dest and src are NULL, returns dest without performing
 *			any operation.
 * @note	Mask array should be at least n bytes long to avoid undefined
 *			behavior.
 *
 * @return	Pointer to the destination memory buffer (dest).
 */
void	*shell_memmove(void *dest, void *src, void *mask, size_t n)
{
	unsigned char	*dst_tmp;
	unsigned char	*src_tmp;

	if (!dest && !src)
		return (dest);
	src_tmp = (unsigned char *) src;
	dst_tmp = (unsigned char *) dest;
	if (dst_tmp > src_tmp)
		iq_movedn((char *) dst_tmp, (char *) src_tmp, (char *) mask, n);
	else
		iq_moveup((char *) dst_tmp, (char *) src_tmp, (char *) mask, n);
	return (dest);
}
