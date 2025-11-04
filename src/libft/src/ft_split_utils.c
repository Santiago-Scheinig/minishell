/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 04:04:09 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:17:45 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Validates that no string in 'base' contains a space.
 *
 *			Iterates through each string in the 'base' array. If any
 *			string contains a space character, the function returns
 *			false (0). Otherwise, returns true (1) if all strings are
 *			valid.
 *
 * @param	base	Array of strings to validate.
 *
 * @return	true (1) if all strings are valid, false (0) otherwise.
 */
int	isset_valid(const char **base)
{
	int	i;
	int	j;

	i = -1;
	while (base[++i])
	{
		j = -1;
		while (base[i][++j])
			if (base[i][j] == ' ')
				return (false);
	}
	return (true);
}

/**
 * @brief	Checks if the start of 's' matches any string in 'base'.
 *
 *			Iterates through each string in the 'base' array. If the
 *			beginning of 's' matches a base string exactly, returns its
 *			length. Otherwise, returns false (0).
 *
 * @param	s		String to check for a base match.
 * @param	base	Array of strings representing the base set.
 *
 * @return	Length of the matching base string, or false (0) if none.
 */
int	isset(const char *s, const char **base)
{
	int	set_len;
	int	i;

	i = -1;
	while (base[++i])
	{
		set_len = ft_strlen(base[i]);
		if (!strncmp(s, base[i], set_len))
			return (set_len);
	}
	return (false);
}

/**
 * @brief	Calculates the length of a word, considering quotes and sets.
 *
 *			Parses the string 's' until the delimiter 'c' or a character
 *			found in 'base' is reached. Quoted substrings are skipped
 *			as single units, so delimiters inside quotes are ignored.
 *
 * @param	s		String to measure the word length from.
 * @param	c		Delimiter character that ends the word.
 * @param	base	Array of strings representing character sets to skip.
 *
 * @return	Length of the word, accounting for quotes and base sets.
 */
int	wrdlen_iqreg(const char *s, char c, const char **base)
{
	int		quote;
	int		i;

	i = 0;
	while (s[i] && s[i] != c && !isset(&s[i], base))
	{
		quote = 0;
		if (s[i] == '\'')
			quote = '\'';
		if (s[i] == '\"')
			quote = '\"';
		i++;
		if (quote && strchr(&s[i], quote))
		{
			while (s[i] != quote)
				i++;
			i++;
		}
	}
	return (i);
}

/**
 * @brief	Frees a partially allocated array of strings and returns NULL.
 *
 *			Iterates through the array of strings up to the specified index,
 *			freeing each allocated string. Finally, frees the array itself
 *			and returns NULL to simplify error handling.
 *
 * @param	wrdstr	Pointer to the array of strings to free.
 * @param	index	Number of elements to free from the array.
 *
 * @note	Use this function when allocation of string arrays fails mid-way.
 *
 * @return	NULL.
 */
void	*memfree(char **wrdstr, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(wrdstr[i]);
		i++;
	}
	free(wrdstr);
	return (NULL);
}
