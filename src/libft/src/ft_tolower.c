/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:46:45 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:18:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Converts an uppercase letter to lowercase.
 *
 *			If 'c' is an uppercase ASCII letter ('A' to 'Z'), returns
 *			the corresponding lowercase letter. All other characters
 *			are returned unchanged.
 *
 * @param	c	Character to convert.
 *
 * @return	Lowercase equivalent if 'c' is uppercase, else 'c' unchanged.
 */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}
