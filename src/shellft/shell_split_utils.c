/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:22:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 21:30:34 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Checks if the string represents a shell token divisor.
 *
 * Evaluates the input string to determine if it matches known shell
 * operators or a space. Returns the corresponding token type. If no
 * match is found, returns WORD.
 *
 * @param str  The string to check for being a divisor/operator.
 *
 * @return  The t_token_type representing the divisor (PIPE, REDIR_IN, etc.)
 *          or WORD if the string is not a divisor.
 */
int	is_divisor(char *str)
{
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	if (!ft_strncmp(str, " ", 1))
		return (END);
	return (WORD);
}

/**
 * @brief Determines the type of a shell token from its string value.
 *
 * Compares the input string to known shell operators and returns the
 * corresponding token type. If no operator matches, the token is
 * classified as a WORD. Empty or NULL strings return END.
 *
 * @param str  The token string to evaluate.
 *
 * @return  The t_token_type of the string (WORD, PIPE, REDIR_IN, etc.).
 */
int	get_token_type(char *str)
{
	if (!str || !str[0])
		return (END);
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	return (WORD);
}

/**
 * @brief Frees a partially allocated array of strings.
 *
 * Frees each string in wrdstr up to the specified index, then frees
 * the array itself.
 *
 * @param wrdstr  Array of strings to free.
 * @param index   Number of elements to free from the array.
 *
 * @return Always returns NULL to simplify error handling in callers.
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

/**
 * @brief Returns the character length of a shell operator token.
 *
 * Determines the length of a token based on its type. Single-character
 * operators (|, <, >) return 1, double-character operators (>>, <<) return 2.
 *
 * @param type 	The token type (t_token_type) to evaluate.
 *
 * @return  Number of characters representing the operator.
 *          Returns 0 if type is not a recognized operator.
 */
int	operator_len(int type)
{
	if (type == PIPE || type == REDIR_IN || type == REDIR_OUT)
		return (1);
	if (type == HEREDOC || type == REDIR_APPEND)
		return (2);
	return (0);
}

/**
 * @brief Computes the length of the next word in a string.
 *
 * Scans the input string s until it reaches a divisor character or the end
 * of the string. Handles quoted sections (single or double quotes) as a
 * single word, skipping over internal characters until the closing quote.
 *
 * @param str Input string to measure the word length from.
 *
 * @note  Uses is_divisor() to identify word boundaries.
 * @note  Quotes are handled properly; the length includes all characters
 *        inside the quotes.
 *
 * @return Length of the next word in characters.
 */
int	word_len(const char *str)
{
	char	*tmp;
	char	quote;
	int		i;

	i = -1;
	tmp = NULL;
	while (str[++i] && !is_divisor((char *) &str[i]))
	{
		if (str[i] == '\'')
		{
			tmp = ft_strchr(&str[i + 1], '\'');
			quote = '\'';
		}
		else if (str[i] == '\"')
		{
			tmp = ft_strchr(&str[i + 1], '\"');
			quote = '\"';
		}
		if (tmp && str[++i])
			while (str[i] && str[i] != quote)
				i++;
		tmp = NULL;
	}
	return (i);
}
