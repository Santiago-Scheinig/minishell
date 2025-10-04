/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 14:43:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:31:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Allocates memory for an array and initializes it to zero.
 *
 * @param   nmemb   Number of elements in the array.
 * @param   size    Size in bytes of each element.
 * @return  Pointer to the allocated memory, or NULL if allocation fails.
 *
 * @note    Returns malloc(0) if size is 0.
 * @note    Checks for overflow: returns NULL if nmemb * size exceeds SIZE_MAX.
 * @note    Memory is zero-initialized using ft_bzero.
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
