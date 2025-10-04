/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:45:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:17:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"
#include "lib/msh_std_utils.h"

/**
 * @brief Splits the input string into an array of token strings.
 *
 * Iterates through the input line, detecting and extracting tokens (words
 * and operators) using their type and length. Each token is duplicated and
 * stored in the output array. Memory is cleaned with memfree() on failure.
 *
 * @param wrdstr		Address of the array to store token strings.
 * @param str			Input string to split into tokens.
 * @param word_amount	Total expected number of tokens.
 *
 * @return	A pointer to the filled token array on success, or NULL on
 *			allocation failure.
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
 * @brief Counts all word tokens in a shell input string.
 *
 * Recursively scans the input string for word tokens, which are sequences
 * of characters not considered shell operators or whitespace. Handles
 * special cases for HEREDOC (<<) and REDIR_APPEND (>>).
 *
 * @param str  The input string to scan for word tokens.
 *
 * @return  The number of word tokens in the string. Returns 0 if the
 *          string is empty or contains no words.
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
 * @brief Counts all operator tokens in a shell input string.
 *
 * Recursively scans the input string for shell operators such as |, >,
 * >>, <<, <, and >, and returns the total number found.
 *
 * @param str  The input string to scan for operator tokens.
 *
 * @return  The number of operator tokens in the string. Returns 0 if the
 *          string is empty or contains no operators.
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
 * @brief Splits a shell input string into tokens.
 *
 * Counts the number of operators and words in the input string, allocates
 * an array of strings, and fills it with each parsed token.
 *
 * @param str  The input shell command string to split.
 *
 * @return  A NULL-terminated array of strings (tokens), or NULL on failure.
 *          If the input string is empty, returns an array with a single NULL.
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
