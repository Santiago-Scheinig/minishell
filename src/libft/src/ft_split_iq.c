/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_iq.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:45:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:07:30 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

static	int	wrdlen(char const *s, char c)
{
	char	*tmp;
	char	quote;
	int		i;

	i = 0;
	tmp = NULL;
	while (s[i] != c && s[i])
	{
		if (s[i] == '\'')
		{
			tmp = ft_strchr(&s[i + 1], '\'');
			quote = '\'';
		}
		if (s[i] == '\"')
		{
			tmp = ft_strchr(&s[i + 1], '\"');
			quote = '\"';
		}
		if (tmp && ++i)
			while (s[i] != quote)
				i++;
		i++;
	}
	return (i);
}

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
	tmp = ft_strchr_iq(s, c);
	if (tmp != NULL && tmp[1])
		count += wrdcount(&tmp[1], c);
	return (count);
}

/**
 * @brief	Splits a string into an array of strings by a delimiter.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c',
 *			while respecting quoted substrings (single and double quotes).
 *			Returns a NULL-terminated array of strings.
 *
 *			Static helpers:
 *
 *				- wrdlen():		Calculates the length of a word considering
 *								quotes.
 *
 *				- wrdcount():	Counts the number of words in 's' considering
 *								quotes.
 *
 * @param	s	String to split.
 * @param	c	Delimiter character.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char	**ft_split_iq(char const *s, char c)
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
			s = ft_strchr_iq(s, c) + 1;
		wrdstr[i] = ft_substr(s, 0, wrdlen(s, c));
		if (!wrdstr[i])
			return (memfree(wrdstr, i));
		s = ft_strchr_iq(s, c) + 1;
		i++;
	}
	wrdstr[i] = (void *)(0);
	return (wrdstr);
}
