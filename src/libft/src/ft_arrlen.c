/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:37 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 04:34:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Calculates the number of elements in a NULL-terminated array.
 *
 * 			Counts the number of non-NULL elements in the array pointed to
 * 			by arr, stopping when a NULL pointer is encountered. Returns 0
 * 			if arr is NULL or if the first element is NULL.
 *
 * @param	arr	Pointer to a NULL-terminated array of pointers.
 *
 * @note	This function works for any array of pointer types, such as
 *			strings or generic pointer lists.
 *
 * @return	Number of elements in the array before the terminating NULL.
 */
int	ft_arrlen(const void **arr)
{
	int			i;

	i = 0;
	if (!arr || !arr[i])
		return (i);
	while (arr[i])
		i++;
	return (i);
}
