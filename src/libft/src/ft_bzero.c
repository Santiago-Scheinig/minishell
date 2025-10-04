/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 18:37:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:30:00 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Sets a memory block to zero.
 *
 * @param   s   Pointer to the memory block to zero out.
 * @param   n   Number of bytes to set to zero.
 *
 * @note    This is a wrapper around ft_memset. If n is 0, nothing is done.
 */
void	ft_bzero(void *s, size_t n)
{
	if (n != 0)
		ft_memset(s, 0, n);
}
