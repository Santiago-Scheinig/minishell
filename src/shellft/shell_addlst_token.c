/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_addlst_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:19:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"
#include "lib/msh_std_utils.h"

/**
 * @brief Applies a mask to a quoted section of a string.
 *
 * Marks the characters inside a quoted section in mask_str:
 * - 'O' for the opening and closing quotes
 * - 'S' for characters inside single quotes
 * - 'D' for characters inside double quotes
 *
 * @param str		Pointer to the input string starting at the opening quote.
 * @param mask_str	Pointer to the mask string where the mask will be applied.
 * @param quote 	The quote character (' or ") to process.
 *
 * @note	Does not allocate memory; modifies mask_str in place.
 *
 * @return	The index offset of the closing quote relative to the start,
 *			useful for advancing in the calling function.
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
 * @brief Creates a mask string for a given token string.
 *
 * Allocates a new string of the same length as str, where each character
 * is marked according to its role:
 * - 'N' for normal characters
 * - 'O' for special characters (;, \, ', ")
 * - characters inside quotes are handled via maskstr_quoted()
 *
 * @param str	Pointer to the input string to mask.
 *
 * @note	Memory is allocated for the mask string; the caller must
 *			free it after use.
 * @note	Uses maskstr_quoted() to handle quoted sections correctly.
 * @note	Returns NULL if memory allocation fails.
 *
 * @return	Pointer to the newly allocated mask string, or NULL on
 *			allocation failure.
 */
static char	*maskstr(char *str)
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
		else if (str[i] == ';' || str[i] == '\\'
			|| str[i] == '\'' || str[i] == '\"')
			mask_str[i] = 'O';
		else
			mask_str[i] = 'N';
	}
	return (mask_str);
}

/**
 * @brief Duplicates a string into a new t_token structure.
 *
 * Allocates a new t_token, sets its string to the provided str, and
 * determines its token type. If the type is WORD, also creates a mask
 * of the string using maskstr().
 *
 * @param str	Pointer to the string to store in the token.
 *
 * @note	Memory for t_token and mask (if applicable) is allocated;
 *			caller is responsible for freeing them.
 * @note	Returns NULL if allocation fails for t_token or mask.
 *
 * @return	Pointer to the newly allocated t_token on success, NULL on
 *			allocation failure.
 */
static t_token	*token_dup(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(t_token));
	new->str = str;
	new->type = get_token_type(str);
	if (new->type == WORD)
	{
		new->mask = maskstr(str);
		if (!new->mask)
			return (NULL);
	}
	return (new);
}

/**
 * @brief Rewrites the content of an existing token.
 *
 * Frees the current string stored in the token and replaces it with
 * the provided string. Updates the mask to match the new string length
 * and sets all characters to 'N'.
 *
 * @param str	Pointer to the new string to store in the token.
 * @param word	Pointer to the t_token to update.
 *
 * @note	Memory for the new string must be managed by the caller.
 * @note	This function never fails.
 */
static void	rewrite_token(char *str, t_token *word)
{
	if (word->str)
		free(word->str);
	word->str = str;
	memset(word->mask, 0, ft_strlen(word->mask));
	memset(word->mask, 'N', ft_strlen(str));
}

/**
 * @brief Adds a new token node or rewrites the current one in a list.
 *
 * Creates a new t_token node with default type WORD and inserts it
 * after the current node in lst_t_token. If rewrite == 0, replaces
 * the content of the current node instead of creating a new node.
 *
 * @param str Pointer to the string to store in the new or replaced token.
 * @param rewrite If 0, replaces the current token content; otherwise
 *			adds a new node after the current one.
 * @param lst_t_token Pointer to the current node in the token list.
 *
 * @note	Memory for the new t_token and its mask is allocated using
 *			token_dup. Caller is responsible for freeing the list.
 * @note	replace_token() never fails.
 *
 * @return	0 on success, 1 on memory allocation failure.
 */
int	shell_addlst_token(char *str, int rewrite, t_list *lst_t_token)
{
	t_token	*aux;
	t_list	*new_node;

	if (!rewrite)
	{
		rewrite_token(str, ((t_token *) lst_t_token->content));
		return (0);
	}
	aux = token_dup(str);
	if (!aux)
		return (1);
	aux->type = WORD;
	memset(aux->mask, 'N', ft_strlen(str));
	new_node = ft_lstnew(aux);
	if (!new_node)
	{
		free(aux);
		return (1);
	}
	new_node->next = lst_t_token->next;
	lst_t_token->next = new_node;
	return (0);
}
