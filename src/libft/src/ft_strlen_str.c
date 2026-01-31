/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:09:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:25:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Calculates length of a string until a given substring.
 *
 *			Iterates through 'str' counting characters until the
 *			substring 'end' is found. Stops when 'end' matches or
 *			when the end of 'str' is reached.
 *
 * @param	str		String to measure.
 * @param	end		Substring marking the stopping point.
 *
 * @note	If 'end' is not found, returns the full string length.
 *
 * @return	Number of characters read before 'end' or end of string.
 */
int	ft_strlen_str(char *str, const char *end)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_strncmp(&str[i], end, strlen(end)))
			break ;
	return (i);
}
