/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_strchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:08:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/29 15:30:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * Searches for the first ocurrance of a space or an operator on a STRING.
 * 
 * @param s The STRING where to find the ocurrance.
 * @return A pointer to a position of the STRING with the first 
 * character ocurrance. If no ocurrance is found, returns NULL. 
 * @note This function will ignore any character coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
const char	*shell_word_strchr(const char *s)
{
	const char	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	while (s[i] && !is_divisor((char *) &s[i]))
	{
		if (s[i] == '\'' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\'');
		if (s[i] == '\"' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\"');
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
	return (&s[i]);
}

/**
 * Searches for the first ocurrance of an operator on a STRING.
 * 
 * @param s The STRING where to find the ocurrance.
 * @return A pointer to a position of the STRING with the first 
 * character ocurrance. If no ocurrance is found, returns NULL. 
 * @note This function will ignore any character coincidences that happen
 * to be inside of single and double quotes (as long they open and close).
 */
const char	*shell_operator_strchr(const char *s)
{
	const char	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	while (s[i] && !get_token_type((char *) &s[i]))
	{
		if (s[i] == '\'' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\'');
		if (s[i] == '\"' && s[i + 1])
			tmp = ft_strchr(&s[i + 1], '\"');
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
	return (&s[i]);
}
