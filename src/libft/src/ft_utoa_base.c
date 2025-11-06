/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:47:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:19:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_nbrlen(size_t n, size_t base_len)
{
	int	count;

	count = 1;
	while (n >= base_len)
	{
		n /= base_len;
		count++;
	}
	return (count);
}

/**
 * @brief	Converts an unsigned integer to a string in a given base.
 *
 *			Converts the number 'n' to a string representation using
 *			the symbols provided in 'base'. The returned string is
 *			dynamically allocated and null-terminated.
 *
 *			Static helpers:
 *
 *				- ft_nbrlen():	Calculates the number of digits needed
 *								to represent 'n' in the given base.
 *
 * @param	n		Unsigned integer to convert.
 * @param	base	String containing all symbols of the target base.
 *
 * @note	The returned string must be freed by the caller.
 * @note	'base' length determines the radix (e.g., 2 for binary, 16 for hex).
 *
 * @return	Dynamically allocated string representing 'n' in the given base,
 *			or NULL if memory allocation fails.
 */
char	*ft_utoa_base(size_t n, const char *base)
{
	size_t	base_len;
	size_t	i;
	char	*str;

	base_len = ft_strlen(base);
	i = ft_nbrlen(n, base_len);
	str = ft_calloc(i + 1, sizeof(char));
	if (!str)
		return (NULL);
	while (n >= base_len)
	{
		str[--i] = base[n % base_len];
		n /= base_len;
	}
	str[--i] = base[n % base_len];
	return (str);
}
