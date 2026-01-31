/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:09:58 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:47:12 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (true);
	return (false);
}

static	int	islow(int c)
{
	if (c >= 'a' && c <= 'z')
		return (true);
	return (false);
}

/**
 * @brief	Checks if a character is alphabetic.
 *
 * 			Determines if 'c' is a letter from 'A' to 'Z' or 'a' to 'z'.
 *
 *			Static helpers:
 *
 *				- isupper():	Checks if 'c' is an uppercase letter.
 *
 *				- islow():		Checks if 'c' is a lowercase letter.
 *
 * @param	c	Character to check.
 *
 * @return	1 if 'c' is alphabetic, 0 otherwise.
 */
int	ft_isalpha(int c)
{
	if (isupper(c) || islow(c))
		return (true);
	return (false);
}
