/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:16:10 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:11:02 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Appends a string to another, respecting buffer size.
 *
 * 			Appends the string 'src' to the end of 'dst', ensuring that
 *			at most 'size - 1' characters are written and the result is
 *			NULL-terminated. Returns the total length of the string it tried
 *			to create.
 *
 * @param	dst	Destination buffer.
 * @param	src	Source string to append.
 * @param	size	Size of the destination buffer.
 *
 * @return	Total length of the string it tried to create (dst + src length).
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	copy_len;
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
		return (size + src_len);
	else if (size >= dst_len + src_len + 1)
		ft_memmove(&dst[dst_len], src, src_len + 1);
	else if (size != 0 && size > dst_len + 1)
	{
		copy_len = size - dst_len - 1;
		if (copy_len > src_len)
			copy_len = src_len;
		ft_memmove(&dst[dst_len], src, copy_len);
		dst[dst_len + copy_len] = '\0';
	}
	return (dst_len + src_len);
}
