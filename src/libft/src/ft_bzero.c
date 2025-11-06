/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:37:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:38:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Sets a block of memory to zero.
 *
 * 			Fills the first 'n' bytes of the memory area pointed to by 's'
 *			with zeros. Does nothing if 'n' is zero.
 *
 * @param	s	Pointer to the memory area to zero.
 * @param	n	Number of bytes to set to zero.
 *
 * @note	This function is a wrapper around ft_memset with value 0.
 */
void	ft_bzero(void *s, size_t n)
{
	if (n != 0)
		ft_memset(s, 0, n);
}
