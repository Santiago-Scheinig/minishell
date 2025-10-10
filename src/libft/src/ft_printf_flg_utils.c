/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flg_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:58:50 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:38:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	isspecifier(char c)
{
	if (c == 'd' || c == 'i' || c == 'u' || c == 'x' || c == 'X')
		return (1);
	else if (c == 'c' || c == 's' || c == 'p' || c == '%')
		return (1);
	return (0);
}

static	const char	*precisioncheck(int *precision, const char *s)
{
	if (*s == '.')
	{
		s++;
		if (ft_isdigit(*s))
			*precision = ft_atoi(s);
		else
			*precision = 0;
	}
	while (ft_isdigit(*s))
		s++;
	return (s);
}

static	const char	*widthcheck(int *width, const char *s)
{
	if (ft_isdigit(*s))
		*width = ft_atoi(s);
	while (ft_isdigit(*s))
		s++;
	return (s);
}

static	const char	*flagcheck(int *flags, const char *s)
{
	while (*s == '-' || *s == '+' || *s == ' ' || *s == '#' || *s == '0')
	{
		if (*s == '-')
			flags[0] = 1;
		if (*s == '+')
			flags[1] = 1;
		if (*s == ' ')
			flags[2] = 1;
		if (*s == '#')
			flags[3] = 1;
		if (*s == '0')
			flags[4] = 1;
		s++;
	}
	return (s);
}

/**
 * @brief	Parses format flags, width, precision, and specifier for printf.
 *
 *			Analyzes the string 's' for standard printf-style flags,
 *			width, precision, and conversion specifiers. Returns an
 *			array representing the parsed values.
 *
 *			Static helpers:
 *
 *				- isspecifier():	Checks if a character is a valid format
 *									specifier.
 *
 *				- flagcheck():		Sets corresponding flags in the array
 *									for '-', '+', ' ', '#', and '0'.
 *
 *				- widthcheck():		Parses field width and stores in array.
 *
 *				- precisioncheck(): Parses precision after '.' and stores
 *									it in the array.
 *
 * @param	s	String containing format specifiers (e.g., "-10.3d").
 *
 * @note	Allocates memory for the flags array using ft_calloc.
 * @note	Flags array structure: [minus, plus, space, hash, zero, width,
 *			precision, specifier].
 *
 * @return	Pointer to an int array with flags, width, precision, and
 *			specifier. Returns NULL on allocation failure or invalid spec.
 */
int	*flagdescriptor(const char *s)
{
	int		*flags;

	flags = ft_calloc(8, sizeof(int));
	if (!flags)
		return (NULL);
	flags[6]--;
	s = flagcheck(flags, s);
	s = widthcheck(&flags[5], s);
	s = precisioncheck(&flags[6], s);
	if (isspecifier(*s))
		flags[7] = (int) *s;
	else
		return (NULL);
	if (flags[7] == '%')
	{
		ft_bzero(flags, 8);
		flags[6]--;
	}
	if (flags[7] == 'u')
	{
		flags[1] = 0;
		flags[2] = 0;
	}
	return (flags);
}
