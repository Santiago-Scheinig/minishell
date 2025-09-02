/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:24:31 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/02 21:05:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * COMMENT PENDING
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
