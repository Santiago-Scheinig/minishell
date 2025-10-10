/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_iqbase.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 01:01:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:07:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

static char	**create_split(int wrds, const char *s, char c, const char **base)
{
	char **wrd_arr;
	int	aux_len;
	int	i;
	int	j;
	
	wrd_arr = ft_calloc(wrds + 1, sizeof(char *));
	if (!wrd_arr)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < wrds)
	{
		while (s[j] == c)
			j++;
		aux_len = isset((&s[j]), base);
		if (!aux_len)
			aux_len = wrdlen_iqreg(&s[j], c, base);
		wrd_arr[i] = ft_substr(&s[j], 0, aux_len);
		if (!wrd_arr[i])
			return(memfree(wrd_arr, i));
		j += aux_len;
	}
	return (wrd_arr);
}

static const char	*strchr_iqreg(const char *s, char c, const char **base)
{
	int			aux;
	int			i;

	i = 0;
	while (s[i])
	{
		aux = isset(&s[i], base);
		if (!aux)
		{
			if (s[i] != c)
				break;
			i++;
		}
		i += aux;
	}
	if (!s[i])
		return (NULL);
	i += wrdlen_iqreg(&s[i], c, base);
	if (s[i] == c)
		i++;
	return (&s[i]);
}

static const char	*strchr_iqset(const char *s, char c, const char **base)
{
	const char	*tmp;
	int			i;

	i = 0;
	while (s[i] && !isset(&s[i], base))
	{
		tmp = NULL;
		if (s[i] == '\'' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\'');
		if (s[i] == '\"' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\"');
		if (tmp)
		{
			s = tmp;
			i = 0;
		}
		i++;
	}
	if (!s[i])
		return (NULL);
	if (s[i] == c)
		return (strchr_iqset(&s[++i], c, base));
	i += isset(&s[i], base);
	return (&s[i]);
}

static int	wrd_count(const char *s, char c, const char **base)
{
	const char	*tmp;
	int			count;

	count = 0;
	if (!s[0])
		return (count);
	tmp = s;
	while(tmp)
	{
		tmp = strchr_iqset(tmp, c, base);
		if (!tmp)
			break;
		count++;
	}
	tmp = s;
	while(tmp)
	{
		tmp = strchr_iqreg(tmp, c, base);
		if (!tmp)
			break;
		count++;
	}
	return (count);
}

/**
 * @brief	Splits a string using a delimiter and a set of special strings.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c',
 *			while respecting quoted substrings, and at each special sets 
 *			string defined in 'base'. Returns a NULL-terminated array of
 *			strings.
 *
 *			Static helpers:
 *
 *				- strchr_iqreg():	Finds the next split position for word
 *									in 's' considering quoted substrings.
 *
 *				- strchr_iqset():	Finds the next split position for base word
 *									in 's' considering quoted substrings.
 *
 *				- wrd_count():		Counts words in 's' based on 'c' and 'base'.
 *
 *				- create_split():	Creates and fills the array of strings.
 *
 * @param	s		String to split.
 * @param	c		Delimiter character.
 * @param	base	Array of special strings to consider for splitting.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char	**ft_split_iqbase(const char *s, char c, const char **base)
{
	char	**wrd_arr;
	int		wrds;

	if (!s || !s[0] || !isset_valid(base))
		return(NULL);
	wrds = wrd_count(s, c, base);
	wrd_arr	= create_split(wrds, s, c, base);
	if (!wrd_arr)
		return (NULL);
	return (wrd_arr);
}
