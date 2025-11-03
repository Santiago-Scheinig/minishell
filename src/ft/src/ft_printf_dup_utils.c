/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dup_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:01:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:17:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Selects the hexadecimal base string based on the specifier.
 *
 *			Returns a pointer to the character set used for converting
 *			numbers to hexadecimal strings. Lowercase is used for 'x'
 *			and 'p', uppercase for 'X'.
 *
 * @param	specifier	Character indicating the desired hex format
 * 						('x', 'X', 'p').
 *
 * @return	Pointer to a string containing hexadecimal digits in the
 *			correct case, or NULL if the specifier is invalid.
 */
const char	*ft_sethexa(int specifier)
{
	const char	*hex_base;

	hex_base = NULL;
	if (specifier == 'x' || specifier == 'p')
		hex_base = "0123456789abcdef";
	if (specifier == 'X')
		hex_base = "0123456789ABCDEF";
	return (hex_base);
}

/**
 * @brief	Prefixes a string pointer with "0x" or "0X" for hex output.
 *
 *			Used to format pointer or hexadecimal strings. Frees the
 *			original string after creating the new prefixed string.
 *
 * @param	ptr		String representing a pointer or hex number. Will be freed.
 * @param	spf		Specifier character ('x' for lowercase, 'X' for uppercase).
 *
 * @return	New string prefixed with "0x" or "0X", or NULL on allocation failure.
 */
char	*ft_ptrdup(void *ptr, int spf)
{
	char	*aux;

	if (spf == 'X')
		aux = ft_strjoin("0X", ptr);
	else
		aux = ft_strjoin("0x", ptr);
	free(ptr);
	return (aux);
}

/**
 * @brief	Creates a new string containing a single character.
 *
 *			Allocates memory for a 2-character string: the given
 *			character and a null terminator.
 *
 * @param	c	Character to duplicate into a new string.
 *
 * @return	Pointer to the newly allocated string, or NULL if allocation fails.
 */
char	*ft_chrdup(char c)
{
	char	*str;

	str = ft_calloc(2, sizeof(char));
	if (!str)
		return (NULL);
	*str = c;
	return (str);
}
