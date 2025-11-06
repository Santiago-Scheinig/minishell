/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:55:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:53:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Searches for a byte in a memory block.
 *
 * 			Scans the first 'n' bytes of the memory area pointed to by 's'
 *			for the first occurrence of byte 'c'.
 *
 * @param	s	Pointer to the memory area to search.
 * @param	c	Byte to search for (converted to unsigned char).
 * @param	n	Number of bytes to examine.
 *
 * @return	Pointer to the first occurrence of 'c' in 's', or NULL if not
 *			found within the first 'n' bytes.
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*s_c;
	unsigned char	c_c;
	size_t			i;

	i = 0;
	s_c = (unsigned char *) s;
	c_c = (unsigned char) c;
	while (i < n)
	{
		if (s_c[i] == c_c)
			return ((void *) &s_c[i]);
		i++;
	}
	return (NULL);
}
