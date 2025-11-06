/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:58:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:59:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Frees a NULL-terminated array of strings.
 *
 * 			Frees each string in the array 'split' and then frees the array
 *			itself. Returns NULL for convenience in assignment.
 *
 * @param	split	Array of strings to free.
 *
 * @return	NULL.
 */
void	*ft_split_free(char **split)
{
	int	i;

	i = -1;
	if (split)
		while (split[++i])
			free(split[i]);
	free(split);
	return (NULL);
}
