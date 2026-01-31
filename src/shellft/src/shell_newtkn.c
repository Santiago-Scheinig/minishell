/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newtkn.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 02:03:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:53:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

static int	is_operator(char c, char *base)
{
	int	i;

	i = -1;
	while (base[++i])
		if (c == base[i])
			return (true);
	return (false);
}

/**
 * @brief	Fills a mask string for characters inside quotes.
 *
 *			Marks the starting and ending quote characters as 'O'. For
 *			single quotes, inner characters are marked as 'S'. For double
 *			quotes, inner characters are marked as 'D'. Returns the number
 *			of characters processed, including the quotes.
 *
 * @param	str			Pointer to the input string with the opening quote.
 * @param	mask_str	Pointer to the corresponding mask string to fill.
 * @param	quote		The quote character ('\'' or '\"') being processed.
 *
 * @return	Number of characters processed in the quoted section.
 */
static int	maskstr_quoted(char *str, char *mask_str, char quote)
{
	int	i;

	i = 0;
	mask_str[i] = 'O';
	while (str[++i] && str[i] != quote)
	{
		if (quote == '\'')
			mask_str[i] = 'S';
		else if (quote == '\"')
			mask_str[i] = 'D';
	}
	mask_str[i] = 'O';
	return (i);
}

/**
 * @brief	Creates a mask string for a shell word to track special characters.
 *
 *			Allocates a new string with the same length as str. Marks each
 *			character as 'N' (normal) or 'O' (operator/special). Skips
 *			over quoted sections and processes them using maskstr_quoted().
 *			Special characters like ';', '\', '\'', and '\"' are marked as 'O'.
 *
 * @param	str	Pointer to the input string to mask.
 *
 * @note	Returns NULL if memory allocation fails.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Pointer to the newly allocated mask string, or NULL on error.
 */
static char	*maskstr(char *str, char *base)
{
	int		i;
	char	quote;
	char	*mask_str;

	mask_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!mask_str)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		quote = 0;
		if (str[i] == '\"' && str[i + 1] && ft_strchr(&str[i + 1], '\"'))
			quote = '\"';
		else if (str[i] == '\'' && str[i + 1] && ft_strchr(&str[i + 1], '\''))
			quote = '\'';
		if (quote)
			i += maskstr_quoted(&str[i], &mask_str[i], quote);
		else if (is_operator(str[i], base))
			mask_str[i] = 'O';
		else
			mask_str[i] = 'N';
	}
	return (mask_str);
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
 * @brief	Creates and initializes a new shell token.
 *
 *			Allocates a t_token structure, zeroes it with ft_memset(), and
 *			assigns the input string. Determines the token type using
 *			get_token_type(). For WORD tokens, duplicates the mask with
 *			mask_dup() to track character masking.
 *
 * @param	str	Pointer to the string representing the token.
 *
 * @note	Returns NULL if memory allocation fails.
 * @note	The mask field is only set for WORD tokens; other types leave it
 * 			NULL.
 *
 * @return	Pointer to the newly allocated t_token structure, or NULL on error.
 */
t_token	*shell_newtkn(char *str, char *base)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->str = str;
	new_token->type = get_token_type(str);
	if (new_token->type == WORD)
	{
		new_token->mask = maskstr(str, base);
		if (!new_token->mask)
			return (NULL);
	}
	return (new_token);
}
