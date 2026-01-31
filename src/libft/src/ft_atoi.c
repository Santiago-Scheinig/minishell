/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:42:32 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:43:26 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	isspace(int c)
{
	if (c == ' ' || c == '\f' || c == '\n'
		|| c == '\r' || c == '\t' || c == '\v')
		return (true);
	return (false);
}

static	int	issign(int c)
{
	if (c == '-')
		return (-1);
	if (c == '+')
		return (1);
	return (0);
}

/**
 * @brief	Converts a string to an integer, handling optional whitespace 
 * 			and sign.
 *
 * 			Parses the string 'nptr' and returns its integer value. Leading
 *			whitespace characters are ignored, and an optional '+' or '-'
 *			sign is handled. Conversion stops at the first non-digit character.
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
 * @note	If 'nptr' is NULL, the function returns 0.
 * @note	Conversion stops at the first non-digit character after optional
 *			whitespace and sign.
 *
 * @return	Integer value represented by the string, with sign applied.
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
