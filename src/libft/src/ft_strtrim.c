/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:43:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:17:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	istrim(char c, char const *set)
{
	int	i;

	i = -1;
	while (set[++i])
		if (set[i] == c)
			return (true);
	return (false);
}

/**
 * @brief	Trims all characters in 'set' from start and end of 's1'.
 *
 *			Allocates and returns a new string with all leading and
 *			trailing characters found in 'set' removed from 's1'.
 *			Returns an empty string if 's1' only contains characters
 *			from 'set'.
 *
 *			Static helpers:
 *
 *				- istrim():		Checks if a character exists in the set.
 *
 * @param	s1		String to trim.
 * @param	set		String containing characters to remove from 's1'.
 *
 * @return	New trimmed string, empty string if all chars trimmed, or
 *			NULL on allocation failure or invalid input.
 */
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*tmp;
	int		s1_len;
	int		i;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (istrim(s1[i], set))
		i++;
	s1_len = ft_strlen(&s1[i]);
	if (s1_len == 0)
		return (ft_strdup(""));
	while (istrim(s1[s1_len + i - 1], set))
		s1_len--;
	tmp = ft_calloc(s1_len + 1, sizeof(char));
	if (!tmp)
		return (NULL);
	ft_strlcat(tmp, &s1[i], s1_len + 1);
	return (tmp);
}
