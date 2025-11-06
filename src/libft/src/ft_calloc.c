/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 14:43:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:39:03 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Allocates memory for an array and initializes it to zero.
 *
 * 			Allocates memory for 'nmemb' elements of 'size' bytes each.
 *			If allocation succeeds, the memory is set to zero using ft_bzero.
 *			Returns NULL on allocation failure or if the requested size is
 *			too large.
 *
 * @param	nmemb	Number of elements to allocate.
 * @param	size	Size of each element in bytes.
 *
 * @note	If either 'nmemb' or 'size' is zero, malloc(0) is called.
 * @note	This function prevents integer overflow in the multiplication.
 *
 * @return	Pointer to the newly allocated zero-initialized memory, or NULL.
 */
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*tmp;

	tmp = NULL;
	if (size == 0)
		return (malloc(0));
	if ((size == 0 && nmemb == 0) || nmemb > __SIZE_MAX__ / size)
		return (tmp);
	tmp = malloc(nmemb * size);
	if (!tmp)
		return (NULL);
	ft_bzero(tmp, nmemb * size);
	return (tmp);
}
