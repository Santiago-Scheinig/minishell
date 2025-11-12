/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstadd_newtkn.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:47:45 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

/**
 * @brief	Replaces the contents of a token with a new string.
 *
 * 			Frees the current token's string and mask, allocates a new
 *			mask for the given string, and fills it with 'N'. Updates the
 *			token's str pointer to the new string.
 *
 * @param	str	New string to store in the token.
 * @param	tkn	Pointer to the token structure to update.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
static int	rewrite_node(char *str, t_token *tkn)
{
	int	str_len;

	str_len = ft_strlen(str);
	if (tkn->str)
		free(tkn->str);
	if (tkn->mask)
		free(tkn->mask);
	tkn->mask = ft_calloc(str_len + 1, sizeof(char));
	if (!tkn->mask)
		return (MSHELL_FAILURE);
	tkn->str = str;
	ft_memset(tkn->mask, 0, str_len + 1);
	ft_memset(tkn->mask, 'N', str_len + 1);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Adds a new token node after the given list node or rewrites it.
 *
 * 			If i is 0, rewrites the current token node using rewrite_node().
 *			Otherwise, creates a new t_token with the string and base,
 *			marks it as WORD, and inserts it after lst_tkn in the list.
 *
 * @param	i		Index flag; 0 = rewrite, otherwise insert new node.
 * @param	str		String to store in the token.
 * @param	base	Base/mask information for the token.
 * @param	lst_tkn	List node after which to add the new token.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
int	shell_lstadd_newtkn(int i, char *str, char *base, t_list *lst_tkn)
{
	t_token	*tkn_new;
	t_list	*new_node;

	if (!i)
		return (rewrite_node(str, ((t_token *) lst_tkn->content)));
	tkn_new = shell_newtkn(str, base);
	if (!tkn_new)
		return (MSHELL_FAILURE);
	tkn_new->type = WORD;
	ft_memset(tkn_new->mask, 'N', ft_strlen(str));
	new_node = ft_lstnew(tkn_new);
	if (!new_node)
	{
		free(tkn_new);
		return (MSHELL_FAILURE);
	}
	new_node->next = lst_tkn->next;
	lst_tkn->next = new_node;
	return (MSHELL_SUCCESS);
}
