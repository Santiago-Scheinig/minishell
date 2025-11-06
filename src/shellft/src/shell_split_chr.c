/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split_chr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 19:08:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/18 17:58:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std_utils.h"

/**
 * @brief	Finds the next word delimiter in a string, ignoring quoted parts.
 *
 *			Iterates through the string until a divisor character is found,
 *			using is_divisor() to detect delimiters. Skips over characters
 *			enclosed in single or double quotes. Returns a pointer to the
 *			first delimiter found, or NULL if none exist.
 *
 * @param	str	Pointer to the input string to search for word delimiters.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 * @note	Returns NULL if no delimiter is found in the string.
 *
 * @return	Pointer to the first delimiter character in str, or NULL if none.
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
 * @brief	Finds the next operator in a string, ignoring quoted sections.
 *
 *			Iterates through the string, skipping characters inside single
 *			or double quotes. Uses get_token_type() to detect operator
 *			characters. Returns a pointer to the first operator found, or
 *			NULL if none exist.
 *
 * @param	str	Pointer to the input string to search for operators.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 * @note	Returns NULL if no operator is found in the string.
 *
 * @return	Pointer to the first operator character in str, or NULL if none.
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
