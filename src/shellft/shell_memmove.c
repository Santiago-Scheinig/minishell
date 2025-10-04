/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_memmove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:58:54 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 20:46:38 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Copies memory from src to dst upward, respecting a mask.
 *
 * Moves bytes from src_tmp to dst_tmp in forward order (from start to end)
 * to handle overlapping memory regions safely. The src_mask array is used
 * to skip or handle certain bytes marked as 'O'.
 *
 * @param dst_tmp	Pointer to the destination memory.
 * @param src_tmp	Pointer to the source memory.
 * @param src_mask	Pointer to a mask array indicating special bytes.
 * @param n			Number of bytes to copy.
 *
 * @note	This function is called internally by shell_memmove when 
 * 			dest <= src.
 * @note	Memory is assumed to be allocated and valid; no bounds checking
 *			is performed beyond n.
 */
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

/**
 * @brief Copies memory from src to dst downward, respecting a mask.
 *
 * Moves bytes from src_tmp to dst_tmp in reverse order (from end to start)
 * to handle overlapping memory regions safely. The src_mask array is used
 * to skip or handle certain bytes marked as 'O'.
 *
 * @param dst_tmp	Pointer to the destination memory.
 * @param src_tmp	Pointer to the source memory.
 * @param src_mask	Pointer to a mask array indicating special bytes.
 * @param len		Number of bytes to copy.
 *
 * @note	This function is called internally by shell_memmove when 
 * 			dest > src.
 * @note	Memory is assumed to be allocated and valid; no bounds checking
 *			is performed beyond len.
 */
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
 * @brief Copies memory from src to dest using a mask.
 *
 * Moves n bytes from src to dest. If dest > src, copies bytes
 * downward to avoid overlap; otherwise, copies upward. The mask
 * array controls special handling of specific bytes 
 * (implementation-dependent).
 *
 * @param dest	Pointer to the destination memory.
 * @param src	Pointer to the source memory.
 * @param mask	Pointer to a mask array affecting how bytes are moved.
 * @param n		Number of bytes to copy.
 *
 * @note	Returns dest even if dest and src are NULL.
 * @note	Uses iq_movedn and iq_moveup internally for handling overlap.
 *
 * @return	Pointer to the destination memory.
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
