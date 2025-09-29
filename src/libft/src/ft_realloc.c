/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:31 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/27 14:46:23 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Reallocates a memory block to a new size, preserving existing contents.
 * 
 * @param ptr Pointer to the previously allocated memory block (may be NULL).
 * @param new_size New size in bytes for the allocation.
 * @param old_size Previous size in bytes of the allocation referenced by ptr.
 * 
 * If ptr is NULL and new_size > 0, behaves like malloc(new_size).
 * If new_size is zero and ptr is non-NULL, frees ptr and returns NULL.
 * Otherwise allocates a new block of new_size bytes, copies the minimum
 * of old_size and new_size bytes from the old block into the new one,
 * frees the old block and returns the new pointer.
 * 
 * @return Pointer to the newly allocated block, or NULL on allocation
 * failure.
 * @note - Caller must ensure old_size correctly represents the original
 * allocation size to avoid copying past bounds.
 * @note - On allocation failure the original ptr is left intact by this
 * implementation.
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
