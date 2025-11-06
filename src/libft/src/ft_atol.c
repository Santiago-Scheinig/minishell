/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:51:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:43:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	long	isspace(long c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (true);
	return (false);
}

static	long	issign(long c)
{
	if (c == '-')
		return (-1);
	if (c == '+')
		return (1);
	return (0);
}

/**
 * @brief	Converts a string to a long integer, handling optional whitespace
 *			and sign.
 *
 * 			Parses the string 'nptr' and returns its long integer value.
 *			Leading whitespace characters are ignored, and an optional '+'
 *			or '-' sign is handled. Conversion stops at the first non-digit
 *			character.
 *
 *			Static helpers:
 *
 *				- isspace():	Checks if a character is a whitespace.
 *
 *				- issign():		Determines if a character is '+' or '-',
 *								returning its sign value.
 *
 * @param	nptr	String representing the number to convert.
 *
 * @note	Conversion stops at the first non-digit character after optional
 *			whitespace and sign.
 *
 * @return	Long integer value represented by the string, with sign applied.
 */
long	ft_atol(const char *nptr)
{
	long	i;
	long	nbr;
	long	sign;

	i = 0;
	nbr = 0;
	sign = 1;
	while (isspace(nptr[i]) == 1)
		i++;
	if (issign(nptr[i]))
		sign = issign(nptr[i++]);
	else if (!ft_isdigit(nptr[i]))
		return (nbr);
	if (ft_isdigit(nptr[i]))
		nbr = nptr[i] - 48;
	else
		return (nbr);
	while (ft_isdigit(nptr[i++ + 1]))
		nbr = (nbr * 10) + (nptr[i] - 48);
	return (nbr * sign);
}
