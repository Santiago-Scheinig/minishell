/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_strchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:08:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:18:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std_utils.h"

/**
 * @brief	Finds the next word delimiter or operator in a string.
 *
 * Scans the input string until a divisor token (space, pipe, or redirection)
 * is found outside of quotes. Quoted substrings are skipped to ensure that
 * delimiters within quotes are ignored.
 *
 * @param str	Input string to scan for the next word boundary.
 *
 * @return	Pointer to the first divisor character found, or NULL if none
 *			exist outside quotes.
 */
const char	*word_strchr(const char *str)
{
	const char	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	while (str[i] && !is_divisor((char *) &str[i]))
	{
		if (str[i] == '\'' && str[i + 1])
			tmp = ft_strchr(&str[i + 1], '\'');
		if (str[i] == '\"' && str[i + 1])
			tmp = ft_strchr(&str[i + 1], '\"');
		if (tmp)
		{
			str = tmp;
			i = 0;
			tmp = NULL;
		}
		i++;
	}
	if (!str[i])
		return (NULL);
	return (&str[i]);
}

/**
 * @brief	Finds the next operator token in a string, skipping quoted text.
 *
 * Iterates through the given string until a shell operator (e.g. |, <, >, <<,
 * >>) is found outside any quotes. Quoted substrings are fully skipped to
 * avoid false positives.
 *
 * @param str	Input string to search for the next operator.
 *
 * @return	Pointer to the first operator character found, or NULL if no
 *			operator exists outside quotes.
 */
const char	*operator_strchr(const char *str)
{
	const char	*tmp;
	int			i;

	i = 0;
	tmp = NULL;
	while (str[i] && !get_token_type((char *) &str[i]))
	{
		if (str[i] == '\'' && str[i + 1])
			tmp = ft_strchr(&str[i + 1], '\'');
		if (str[i] == '\"' && str[i + 1])
			tmp = ft_strchr(&str[i + 1], '\"');
		if (tmp)
		{
			str = tmp;
			i = 0;
			tmp = NULL;
		}
		i++;
	}
	if (!str[i])
		return (NULL);
	return (&str[i]);
}
