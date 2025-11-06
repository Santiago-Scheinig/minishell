/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 13:48:27 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:13:16 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Searches for a substring within a limited length of a string.
 *
 * 			Searches for the first occurrence of 'little' in 'big', but
 *			only within the first 'len' characters of 'big'. Returns a
 *			pointer to the beginning of 'little' in 'big', or NULL if not
 *			found.
 *
 * @param	big	String to search within.
 * @param	little	Substring to search for.
 * @param	len	Maximum number of characters to search in 'big'.
 *
 * @return	Pointer to the first occurrence of 'little' in 'big', or NULL.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		little_len;

	if (!big)
		return (NULL);
	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *) big);
	i = 0;
	while (i < len)
	{
		if (!big[i])
			return (NULL);
		if (big[i] == little[0] && len >= little_len + i)
		{
			if (ft_strncmp(&big[i], &little[0], little_len) == 0)
				return ((char *) &big[i]);
		}
		i++;
	}
	return (NULL);
}
