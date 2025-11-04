/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:21:20 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:07:58 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Finds the first occurrence of a character in a string.
 *
 * 			Searches for the first occurrence of the character 'c' in the
 *			string 's'. Returns a pointer to the character found or NULL if
 *			not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to search for.
 *
 * @return	Pointer to the first occurrence of 'c' in 's', or NULL if not found.
 */
char	*ft_strchr(const char *s, int c)
{
	size_t	i;
	char	c_c;

	if (!s)
		return (NULL);
	i = 0;
	c_c = (char) c;
	while (s[i])
	{
		if (s[i] == c_c)
			return ((char *) &s[i]);
		i++;
	}
	if (s[i] == c_c)
		return ((char *) &s[i]);
	return (NULL);
}
