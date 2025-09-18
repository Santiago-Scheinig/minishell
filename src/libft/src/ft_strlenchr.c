/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlenchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 17:47:41 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 20:02:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Counts the amount of characters on a STRING until the CHAR.
 * 
 * @param str The STRING to count characters from.
 * @param c the CHAR until gona count.
 * @return An INT with the amount of characters counted.
 * @note This function doesn't count the '\0' character.
 */
int	ft_strlenchr(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}
