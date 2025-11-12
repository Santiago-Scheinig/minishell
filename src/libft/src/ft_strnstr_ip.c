/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr_ip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:57:51 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:26:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	jump_parenthesis(const char *str, int i)
{
	if (str[i] == '(')
	{
		while (str[i] && str[i] != ')')
			i++;
		return (++i);
	}
	return (i);
}

static int	jump_quotes(const char *str, int i)
{
	int	quote;

	quote = 0;
	if (str[i] == '\'')
		quote = '\'';
	if (str[i] == '\"')
		quote = '\"';
	if (quote)
	{
		i++;
		while (str[i] && str[i] != quote)
			i++;
	}
	return (i);
}

/**
 * @brief	Finds a substring in a string, ignoring quotes and
 *			parentheses.
 *
 *			Searches for the substring 'little' in the string 'big'
 *			up to 'len' characters, skipping over segments inside
 *			parentheses or quotes. Behaves like ft_strnstr() but
 *			with syntax-aware parsing.
 *
 *			Static helpers:
 *
 *				- jump_parenthesis():	Skips text inside '()'.
 *				- jump_quotes():		Skips text inside quotes.
 *
 * @param	big		String to search within.
 * @param	little	Substring to find.
 * @param	len		Maximum number of characters to search.
 *
 * @note	Search stops when 'little' is found outside ignored
 *			sections or end of string is reached.
 *
 * @return	Pointer to first match of 'little' in 'big', or NULL
 *			if not found.
 */
char	*ft_strnstr_ip(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		little_len;

	if (!big)
		return (NULL);
	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *) big);
	i = 0;
	while (i < len)
	{
		if (!big[i])
			return (NULL);
		i = jump_quotes(big, i);
		i = jump_parenthesis(big, i);
		if (big[i] == little[0] && len >= little_len + i)
		{
			if (ft_strncmp(&big[i], &little[0], little_len) == 0)
				return ((char *) &big[i]);
		}
		i++;
	}
	return (NULL);
}
