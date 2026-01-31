/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 20:49:13 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:14:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Finds the last occurrence of a character in a string.
 *
 * 			Searches for the last occurrence of the character 'c' in the
 *			string 's'. Returns a pointer to this character, or NULL if it
 *			is not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to locate.
 *
 * @return	Pointer to the last occurrence of 'c' in 's', or NULL if none.
 */
char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*tmp;
	char	c_c;

	i = 0;
	c_c = (char) c;
	tmp = NULL;
	while (s[i])
	{
		if (s[i] == c_c)
			tmp = (char *) &s[i];
		i++;
	}
	if (s[i] == c_c)
		tmp = (char *) &s[i];
	return ((char *) tmp);
}
