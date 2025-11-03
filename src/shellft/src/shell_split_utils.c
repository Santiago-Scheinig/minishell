/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:22:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/20 20:14:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Determines if the string starts with a word delimiter or operator.
 *
 *			Checks the input string against known shell operators: PIPE ("|"),
 *			REDIR_APPEND (">>"), HEREDOC ("<<"), REDIR_IN ("<"), REDIR_OUT
 *			(">"), and space (" ") as a word delimiter. Returns WORD if none
 *			match.
 *
 * @param	str	Pointer to the string to analyze.
 *
 * @note	The returned values correspond to predefined shell token constants.
 *
 * @return	Integer representing the type of delimiter or operator, or WORD.
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
 * @brief	Identifies the type of a shell token from a string.
 *
 *			Checks the input string against known shell operators:
 *			PIPE ("|"), REDIR_APPEND (">>"), HEREDOC ("<<"), REDIR_IN ("<"),
 *			and REDIR_OUT (">"). Returns WORD for non-operator strings and
 *			END for NULL or empty strings.
 *
 * @param	str	Pointer to the string to analyze.
 *
 * @note	Token types must match the predefined constants used in the shell.
 *
 * @return	Integer representing the token type (END, PIPE, REDIR_APPEND,
 *			HEREDOC, REDIR_IN, REDIR_OUT, or WORD).
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
 * @brief	Returns the length in characters of a given shell operator type.
 *
 *			Checks the operator type and returns 1 for single-character
 *			operators (PIPE, REDIR_IN, REDIR_OUT) and 2 for double-character
 *			operators (HEREDOC, REDIR_APPEND). Returns 0 for unknown types.
 *
 * @param	type	Integer representing the token/operator type.
 *
 * @note	Operator types must match the predefined constants used by the shell.
 *
 * @return	Length of the operator in characters, or 0 if type is unknown.
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
 * @brief	Calculates the length of the next word, ignoring quoted parts.
 *
 *			Iterates through the string until a divisor character is reached,
 *			skipping characters inside single or double quotes. Properly handles
 *			quoted sections to include them in the word length.
 *
 * @param	str	Pointer to the input string to measure.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 *
 * @return	Length of the next word in characters.
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
