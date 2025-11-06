/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:50:20 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:11:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Calculates the length of a string.
 *
 * 			Counts the number of characters in the string 's' until the
 *			NULL terminator. Returns 0 if 's' is NULL.
 *
 * @param	s	String to measure.
 *
 * @return	Number of characters in 's', excluding the NULL terminator.
 */
size_t	ft_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}
