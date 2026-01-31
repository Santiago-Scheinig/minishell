/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_iq.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 18:15:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:18:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_quote(char c)
{
	int	i;

	i = -1;
	if (c == '\'')
		return ('\'');
	if (c == '\"')
		return ('\"');
	return (0);
}

/**
 * @brief	Finds a character in a string, ignoring quoted substrings.
 *
 * 			Searches for the first occurrence of character 'c' in the
 *			string 's', skipping characters inside single or double quotes.
 *			Returns a pointer to the character found or NULL if not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to search for.
 *
 * @return	Pointer to the first occurrence of 'c' outside quotes, or NULL.
 */
char	*ft_strchr_iq(const char *s, char c)
{
	char	*tmp;
	int		i;
	int		quote;

	i = 0;
	tmp = NULL;
	while (s[i] && s[i] != c)
	{
		quote = is_quote(s[i]);
		if (quote && s[i + 1])
			tmp = ft_strchr(&s[i + 1], quote);
		if (tmp)
		{
			s = tmp;
			i = 0;
			tmp = NULL;
		}
		i++;
	}
	if (!s[i])
		return (NULL);
	return ((char *) &s[i]);
}
