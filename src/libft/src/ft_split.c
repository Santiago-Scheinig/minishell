/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:49:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:05:48 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

static	int	wrdcount(char const *s, char c)
{
	char	*tmp;
	int		count;

	count = 0;
	if (c == 0)
		return (1);
	if (s[0] == 0)
		return (0);
	if (s[0] != c)
		count++;
	tmp = ft_strchr(s, c);
	if (tmp != NULL && tmp[1])
		count += wrdcount(&tmp[1], c);
	return (count);
}

static	int	wrdlen(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	return (i);
}

/**
 * @brief	Splits a string into an array of strings using a delimiter.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c'.
 *			Returns a NULL-terminated array of strings.
 *
 *			Static helpers:
 *
 *				- wrdcount():	Counts the number of words in 's' separated
 *								by 'c'.
 *
 *				- wrdlen():		Calculates the length of a word in 's' until
 *								the next 'c' or string end.
 *
 * @param	s	String to split.
 * @param	c	Delimiter character.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char	**ft_split(char const *s, char c)
{
	char	**wrdstr;
	int		w;
	int		i;

	if (!s)
		return (NULL);
	if (s[0] == 0)
		return (ft_calloc(1, sizeof(char *)));
	i = 0;
	w = wrdcount(s, c);
	wrdstr = (char **) malloc((w + 1) * sizeof(char *));
	if (!wrdstr)
		return (NULL);
	while (i < w)
	{
		while (s[0] == c)
			s = ft_strchr(s, c) + 1;
		wrdstr[i] = ft_substr(s, 0, wrdlen(s, c));
		if (!wrdstr[i])
			return (memfree(wrdstr, i), NULL);
		s = ft_strchr(s, c) + 1;
		i++;
	}
	wrdstr[i] = (void *)(0);
	return (wrdstr);
}
