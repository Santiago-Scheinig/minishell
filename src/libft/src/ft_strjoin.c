/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 19:33:04 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:10:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Concatenates two strings into a newly allocated string.
 *
 * 			Creates a new string that contains the contents of 's1' followed
 *			by the contents of 's2'. Memory is allocated for the new string.
 *
 * @param	s1	First string.
 * @param	s2	Second string.
 *
 * @return	Pointer to the concatenated string, or NULL on allocation failure.
 */
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	int		s1_len;
	int		s2_len;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ptr = ft_calloc(s1_len + s2_len + 1, 1);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, s1, s1_len);
	ft_strlcat(ptr, s2, s1_len + s2_len + 1);
	return (ptr);
}
