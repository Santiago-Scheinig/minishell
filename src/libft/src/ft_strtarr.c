/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtarr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 18:25:23 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/02 21:05:37 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * COMMENT PENDING
 */
char	**ft_strtarr(char **arr, char *str)
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
