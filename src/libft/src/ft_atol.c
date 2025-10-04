/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 14:51:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:29:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Checks if a character is a whitespace character.
 *
 * @param   c   The character to check.
 *
 * @return  1 if the character is a space, tab, newline, vertical tab,
 *          form feed, or carriage return; 0 otherwise.
 *
 * @note    Used by parsing functions like ft_atol to skip leading whitespace.
 */
static	long	ft_isspace(long c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

/**
 * @brief   Checks if a character is a sign indicator ('+' or '-').
 *
 * @param   c   The character to check.
 *
 * @return  -1 if c is '-', 1 if c is '+', 0 otherwise.
 *
 * @note    Used by ft_atol to determine the number's sign.
 */
static	long	ft_issign(long c)
{
	if (c == '-')
		return (-1);
	if (c == '+')
		return (1);
	return (0);
}

/**
 * @brief   Converts a string to a long integer.
 *
 * @param   nptr    The string containing the number.
 *
 * @return  The converted long integer. Returns 0 if nptr is NULL
 *          or if the first non-whitespace, optional-sign character
 *          is not a digit.
 *
 * @note    Skips leading whitespace, handles an optional single
 *          '+' or '-' sign, and parses consecutive digits until
 *          a non-digit is found.
 */
long	ft_atol(const char *nptr)
{
	long	i;
	long	nbr;
	long	sign;

	i = 0;
	nbr = 0;
	sign = 1;
	while (ft_isspace(nptr[i]) == 1)
		i++;
	if (ft_issign(nptr[i]))
		sign = ft_issign(nptr[i++]);
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
