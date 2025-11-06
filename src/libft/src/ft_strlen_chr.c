/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_chr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:47:41 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 05:12:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Calculates the length of a string until a given character.
 *
 * 			Counts the number of characters in 'str' until character 'c'
 *			or the NULL terminator is found.
 *
 * @param	str	String to measure.
 * @param	c	Character to stop counting at.
 *
 * @return	Number of characters before 'c' or end of string.
 */
int	ft_strlen_chr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
