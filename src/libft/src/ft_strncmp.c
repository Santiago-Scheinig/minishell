/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 11:58:18 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:12:56 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Compares up to 'n' characters of two strings.
 *
 * 			Compares the strings 's1' and 's2' up to 'n' characters or until
 *			a difference or NULL terminator is found. Returns the difference
 *			between the first differing characters.
 *
 * @param	s1	First string to compare.
 * @param	s2	Second string to compare.
 * @param	n	Maximum number of characters to compare.
 *
 * @return	Integer less than, equal to, or greater than zero if 's1' is
 *			less than, equal to, or greater than 's2'.
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	if (!s1 || !s2)
		return (1);
	i = 0;
	while (i < n && s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (i == n)
		i--;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}
