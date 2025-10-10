/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:26:08 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:11:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Copies a string into a buffer, respecting its size.
 *
 * 			Copies up to 'size - 1' characters from 'src' into 'dst' and
 *			NULL-terminates the result if 'size' is not zero. Returns the
 *			length of 'src'.
 *
 * @param	dst	Destination buffer.
 * @param	src	Source string to copy.
 * @param	size	Size of the destination buffer.
 *
 * @return	Length of the source string 'src'.
 */
size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	src_len;

	if (!dst || !src)
		return (0);
	src_len = ft_strlen(src);
	if (size >= src_len + 1)
		ft_memmove(dst, src, src_len + 1);
	else if (dst && size != 0)
	{
		ft_memmove(dst, src, size - 1);
		dst[size - 1] = 0;
	}
	return (src_len);
}
