/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_ip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:53:18 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:24:53 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_parenthesis(char c)
{
	int	i;

	i = -1;
	if (c == '(')
		return (')');
	return (0);
}

/**
 * @brief	Searches for a character in a string, ignoring sections
 *			enclosed in parentheses.
 *
 *			Scans the string 's' for the first occurrence of 'c',
 *			skipping any text found inside parentheses. Uses
 *			is_parenthesis() to detect and skip balanced pairs.
 *
 *			Static helpers:
 *
 *				- is_parenthesis():	Detects '(' and returns ')'
 *									to identify matching pairs.
 *
 * @param	s	String to search within.
 * @param	c	Character to find.
 *
 * @note	Returns a pointer to the first valid match outside
 *			of any parentheses, or NULL if not found.
 *
 * @return	Pointer to the matching character in 's', or NULL
 *			if not found.
 */
char	*ft_strchr_ip(const char *s, char c)
{
	char	*tmp;
	int		i;
	int		parenthesis;

	i = 0;
	tmp = NULL;
	while (s[i] && s[i] != c)
	{
		parenthesis = is_parenthesis(s[i]);
		if (parenthesis && s[i + 1])
			tmp = ft_strchr(&s[i + 1], parenthesis);
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
