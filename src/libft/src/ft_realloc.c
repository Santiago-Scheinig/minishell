/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:31 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 14:08:57 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Resizes a memory block, preserving its content.
 *
 * 			Allocates a new memory block of size 'new_size', copies the
 *			content from 'ptr' up to the smaller of 'old_size' or
 *			'new_size', and frees the old block. Handles NULL pointers and
 *			zero-size allocations.
 *
 * @param	ptr		Pointer to the old memory block.
 * @param	new_size	New size in bytes for the memory block.
 * @param	old_size	Old size in bytes of the memory block.
 *
 * @return	Pointer to the new memory block, or NULL on allocation failure.
 */
void	*ft_realloc(void *ptr, size_t new_size, size_t old_size)
{
	void	*new_alloc;

	if (ptr && !new_size)
		free(ptr);
	if (!ptr && new_size)
		return (malloc(new_size));
	new_alloc = malloc(new_size);
	if (!new_alloc)
		return (NULL);
	if (old_size < new_size)
		ft_memmove(new_alloc, (const void *) ptr, old_size);
	else
		ft_memmove(new_alloc, (const void *) ptr, new_size);
	free(ptr);
	return (new_alloc);
}
