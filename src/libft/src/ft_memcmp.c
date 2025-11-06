/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:39:12 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:53:53 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Compares two memory blocks byte by byte.
 *
 * 			Compares the first 'n' bytes of the memory areas 's1' and 's2'.
 *			Returns the difference between the first pair of differing bytes.
 *
 * @param	s1	Pointer to the first memory block.
 * @param	s2	Pointer to the second memory block.
 * @param	n	Number of bytes to compare.
 *
 * @note	If either 's1' or 's2' is NULL, returns 'n'.
 *
 * @return	Integer less than, equal to, or greater than zero if 's1' is
 *			respectively less than, equal to, or greater than 's2'.
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1_c;
	unsigned char	*s2_c;
	size_t			i;

	i = 0;
	if (!s1 || !s2)
		return (n);
	s1_c = (unsigned char *) s1;
	s2_c = (unsigned char *) s2;
	while (i < n && s1_c[i] == s2_c[i])
		i++;
	if (i == n)
		i--;
	return ((unsigned char) s1_c[i] - (unsigned char) s2_c[i]);
}
