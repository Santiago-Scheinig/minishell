/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 17:24:31 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:09:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Creates a duplicate of a string.
 *
 * 			Allocates memory and copies the content of the string 's' into
 *			the new memory. Returns a pointer to the duplicated string.
 *
 * @param	s	String to duplicate.
 *
 * @return	Pointer to the duplicated string, or NULL on allocation failure.
 */
char	*ft_strdup(const char *s)
{
	char	*str;
	int		str_len;

	if (!s)
		return (NULL);
	str_len = ft_strlen(s);
	str = malloc(str_len + 1);
	if (!str)
		return (NULL);
	ft_bzero(str, str_len + 1);
	ft_memcpy(str, s, str_len);
	return (str);
}
