/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:45:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/18 17:58:18 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

/**
 * @brief	Fills an array with words and operators from a command string.
 *
 *			Iterates through the input string, skipping spaces. Determines
 *			whether the next token is a word or an operator using
 *			get_token_type(). Extracts the token with ft_substr() and stores
 *			it in the array. Continues until all tokens are processed.
 *
 * @param	wrdstr		Pointer to the array of strings to fill.
 * @param	str			Pointer to the input command string.
 * @param	word_amount	Number of tokens to extract (words + operators).
 *
 * @note	Returns NULL if memory allocation fails; partially allocated
 *			strings are freed using memfree().
 * @note	The returned array is NULL-terminated.
 *
 * @return	Pointer to the filled array of strings, or NULL on error.
 */
static char	***create_split(char ***wrdstr, const char *str, int word_amount)
{
	t_token_type	type;
	int				token_len;
	int				i;

	i = -1;
	while (++i < word_amount)
	{
		while ((*str) == ' ')
			str++;
		type = get_token_type((char *)&str[0]);
		if (!type)
			token_len = word_len((char *) str);
		else
			token_len = operator_len(type);
		(*wrdstr)[i] = ft_substr(str, 0, token_len);
		if (!((*wrdstr)[i]))
			return (memfree((*wrdstr), i));
		while (token_len)
		{
			token_len--;
			str++;
		}
	}
	(*wrdstr)[i] = (void *)(0);
	return (wrdstr);
}

/**
 * @brief	Counts the number of word tokens in a shell command string.
 *
 *			Iterates through the string, ignoring operators and spaces.
 *			Uses get_token_type() and word_strchr() to identify words,
 *			and recursively counts additional words in the remainder of
 *			the string.
 *
 * @param	str	Pointer to the input command string.
 *
 * @note	Words inside quotes are counted as a single token.
 * @note	Returns 0 if the string is empty or contains no words.
 *
 * @return	Number of word tokens in the input string.
 */
static int	word_count(const char *str)
{
	const char	*tmp;
	int			i;
	int			type;
	int			count;

	i = 0;
	count = 0;
	if (str[0] == 0)
		return (0);
	type = get_token_type((char *) &str[0]);
	if (!type && str[0] != ' ' && str[0] != '\n')
		count++;
	tmp = word_strchr(&str[0]);
	type = get_token_type((char *) &tmp[0]);
	if (type == HEREDOC || type == REDIR_APPEND)
		i++;
	if (tmp != NULL && tmp[i + 1])
		count += word_count(&tmp[i + 1]);
	return (count);
}

/**
 * @brief	Counts the number of operator tokens in a shell command string.
 *
 *			Checks the current position in the string for an operator using
 *			get_token_type(). Increments the count for each operator found
 *			and recursively searches for additional operators in the remainder
 *			of the string using operator_strchr().
 *
 * @param	str	Pointer to the input command string.
 *
 * @note	Handles both single-character and double-character operators.
 * @note	Returns 0 if the string is empty or contains no operators.
 *
 * @return	Number of operator tokens in the input string.
 */
static int	operator_count(const char *str)
{
	const char	*tmp;
	int			i;
	int			type;
	int			count;

	i = 0;
	count = 0;
	if (str[i] == 0)
		return (0);
	type = get_token_type((char *) &str[i]);
	if (type > WORD && type < END)
		count++;
	if (type && type != END)
		i++;
	if (type == REDIR_APPEND || type == HEREDOC)
		i++;
	tmp = operator_strchr(&str[i]);
	if (tmp != NULL)
		count += operator_count(&tmp[0]);
	return (count);
}

/**
 * @brief	Splits a shell command string into words and operators.
 *
 *			Counts the number of operators and words in the input string,
 *			then allocates an array of strings. Uses create_split() to fill
 *			the array with the individual words and operator tokens.
 *
 * @param	str	Pointer to the input command string to split.
 *
 * @note	Returns an empty array if the input string is empty.
 * @note	Returns NULL if memory allocation fails.
 * @note	The returned array and its strings must be freed by the caller.
 *
 * @return	Pointer to the newly allocated array of strings, or NULL on error.
 */
char	**shell_split(const char *str)
{
	char	**wrdstr;
	int		word_amount;
	int		i;

	if (!str)
		return (NULL);
	if (str[0] == 0)
		return (ft_calloc(1, sizeof(char *)));
	i = 0;
	word_amount = operator_count(str);
	word_amount += word_count(str);
	wrdstr = (char **) malloc((word_amount + 1) * sizeof(char *));
	if (!wrdstr)
		return (NULL);
	if (create_split(&wrdstr, str, word_amount) == NULL)
		return (NULL);
	return (wrdstr);
}
