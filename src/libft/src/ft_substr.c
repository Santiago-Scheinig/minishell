/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:53:07 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:17:37 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Extracts a substring from a given string.
 *
 *			Allocates and returns a new string that is a substring of
 *			's', starting at index 'start' and up to 'len' characters.
 *			If 'start' is beyond the end of 's', returns an empty string.
 *			If 'len' exceeds the remaining string length, it is reduced.
 *
 * @param	s		Original string to extract from.
 * @param	start	Starting index for the substring.
 * @param	len		Maximum number of characters to include.
 *
 * @note	Returns NULL if 's' is NULL or memory allocation fails.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Newly allocated substring, empty string if start > strlen(s),
 *			or NULL on allocation failure.
 */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(&s[start]))
		len = ft_strlen(&s[start]);
	str = ft_calloc(len + 1, 1);
	if (!str)
		return (NULL);
	ft_strlcpy(str, &s[start], len + 1);
	return (str);
}
