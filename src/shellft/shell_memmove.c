/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_memmove.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:58:54 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/23 16:07:48 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * Moves n bytes from a src VOID pointer into a dest VOID pointer, specific
 * when dst_tmp pointer is in a lower memory possition than src_tmp.
 * @param dst_tmp The VOID pointer where to move bytes into.
 * @param src_tmp The VOID pointer where to move bytes from.
 * @param len The amount of bytes to be moved.
 * @note Operators such as '\'' and '\"' are ignored, and as such, aren't
 * moved to dst. This mimics the quote removal behaivor from bash-shell.
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
			continue;
		}
		if (i < n)
			dst_tmp[j] = src_tmp[i];
		i++;
		j++;
	}
}

/**
 * Moves n bytes from a src VOID pointer into a dest VOID pointer, specific
 * when dst_tmp pointer is in a higher memory possition than src_tmp.
 * @param dst_tmp The VOID pointer where to move bytes into.
 * @param src_tmp The VOID pointer where to move bytes from.
 * @param len The amount of bytes to be moved.
 * @note Operators such as '\'' and '\"' are ignored, and as such, aren't
 * moved to dst. This mimics the quote removal behaivor from bash-shell.
 */
static void	iq_movedown(char *dst_tmp, char *src_tmp, char *src_mask, size_t len)
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
 * Moves n bytes from a src VOID pointer into a dest VOID pointer.
 * @param dest The VOID pointer where to move bytes into.
 * @param src The VOID pointer where to move bytes from.
 * @param n The amount of bytes to be moved.
 * @return The VOID pointer to dest.
 * @note This function modifies the dest VOID pointer and, additionally,
 * checks if the memory position of src is close to dest to avoid loosing
 * information during the movement.
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
		iq_movedown((char *) dst_tmp,(char *) src_tmp, (char *) mask, n);
	else
		iq_moveup((char *) dst_tmp,(char *) src_tmp, (char *) mask, n);
	return (dest);
}
