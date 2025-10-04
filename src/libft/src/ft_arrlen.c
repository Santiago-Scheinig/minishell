/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:37 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 22:27:22 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Calculates the number of elements in a NULL-terminated array.
 *
 *          This function iterates through the given array of pointers until
 *          it reaches a NULL terminator, counting the number of valid
 *          elements. It can be used for arrays of strings or any other
 *          pointer-based structures that follow the NULL-termination
 *          convention.
 *
 * @param   arr     Pointer to a NULL-terminated array of pointers. The array
 *                  elements may point to any type of data.
 *
 * @note    The function performs no validation on the contents of the array
 *          elements — it only counts how many non-NULL entries appear before
 *          the NULL terminator.
 *
 * @return  The number of non-NULL elements in the array.
 *          Returns 0 if @p arr is NULL or if the first element is NULL.
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
