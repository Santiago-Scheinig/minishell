/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:45:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:18:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Converts a lowercase letter to uppercase.
 *
 *			If 'c' is a lowercase ASCII letter ('a' to 'z'), returns
 *			the corresponding uppercase letter. All other characters
 *			are returned unchanged.
 *
 * @param	c	Character to convert.
 *
 * @return	Uppercase equivalent if 'c' is lowercase, else 'c' unchanged.
 */
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
