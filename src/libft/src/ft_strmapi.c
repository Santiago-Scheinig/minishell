/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:07:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:12:37 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Creates a new string by applying a function to each character.
 *
 * 			Allocates a new string where each character is the result of
 *			applying the function 'f' to the corresponding character of 's'.
 *
 * @param	s	String to map.
 * @param	f	Function to apply to each character and its index.
 *
 * @return	New string with mapped characters, or NULL on allocation failure.
 */
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*str;
	int		len;
	int		i;

	i = 0;
	len = ft_strlen(s);
	str = ft_calloc(len + 1, 1);
	if (!str)
		return (NULL);
	while (s[i])
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	return (str);
}
