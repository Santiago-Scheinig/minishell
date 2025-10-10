/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_arrdd_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:23 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 05:15:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Appends a string to a dynamically allocated array of strings.
 *
 * 			Expands the array 'arr' to hold one more string and adds 'str'
 *			at the end. The resulting array is NULL-terminated.
 *
 * @param	arr	Array of strings to append to.
 * @param	str	String to add to the array.
 *
 * @return	New array including 'str', or NULL on allocation failure.
 */
char	**ft_arradd_str(char **arr, char *str)
{
	char	**tmp;
	int		len;
	size_t	old_size;
	size_t	new_size;

	len = ft_arrlen((const void **) arr) + 1;
	old_size = sizeof(char *) * len;
	new_size = old_size + sizeof(char *);
	tmp = ft_realloc((void *) arr, new_size, old_size);
	if (!tmp)
	{
		ft_split_free(arr);
		return (NULL);
	}
	tmp[len] = str;
	tmp[len + 1] = NULL;
	return (tmp);
}
