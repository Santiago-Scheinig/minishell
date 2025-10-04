/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:42:32 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:28:51 by sscheini         ###   ########.fr       */
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
 * @note    Used by parsing functions like ft_atoi to skip leading whitespace.
 */
static	int	ft_isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

/**
 * @brief   Determines if a character is a sign indicator.
 *
 * @param   c   The character to check.
 *
 * @return  -1 if c is '-', 1 if c is '+', 0 otherwise.
 *
 * @note    Used by functions like ft_atoi to handle signed numbers.
 */
static	int	ft_issign(int c)
{
	if (c == '-')
		return (-1);
	if (c == '+')
		return (1);
	return (0);
}

/**
 * @brief   Converts a string to an integer (base 10).
 *
 *          Skips leading whitespace, handles optional '+' or '-' signs,
 *          and converts consecutive digits into an integer value.
 *
 * @param   nptr    The input string to convert.
 *
 * @return  The integer value represented by the string.
 *          Returns 0 if the string is NULL or contains no digits.
 *
 * @note    Behavior for integer overflow is undefined.
 * @note    Only consecutive digits after optional sign are converted;
 *          any trailing non-digit characters are ignored.
 */
int	ft_atoi(const char *nptr)
{
	int	i;
	int	nbr;
	int	sign;

	if (!nptr)
		return (0);
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
