/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstadd_newtkn.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 07:20:36 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

static void	rewrite_node(char *str, t_token *tkn)
{
	if (tkn->str)
		free(tkn->str);
	tkn->str = str;
	ft_memset(tkn->mask, 0, ft_strlen(tkn->mask));
	ft_memset(tkn->mask, 'N', ft_strlen(str));
}

/**
 * @brief	Adds a new token to a token list or rewrites an existing one.
 *
 *			If rewrite is false, updates the content of the given list node
 *			using rewrite_node(). Otherwise, allocates a new t_token, sets
 *			its type to WORD, initializes its mask, wraps it in a new list
 *			node, and inserts it after the provided node in lst_tkn.
 *
 * @param	rewrite	Boolean flag; if 0, rewrites existing node, else adds.
 * @param	str		String content of the token to add or rewrite.
 * @param	lst_tkn	Pointer to the existing token list node.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails during token 
 * 			or node creation.
 * @note	The new node is inserted immediately after lst_tkn.
 * @note	The mask field is initialized to 'N' for each character in str.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation failure.
 */
int	shell_lstadd_newtkn(int rewrite, char *str, t_list *lst_tkn)
{
	t_token	*tkn_new;
	t_list	*new_node;

	if (!rewrite)
	{
		rewrite_node(str, ((t_token *) lst_tkn->content));
		return (MSHELL_SUCCESS);
	}
	tkn_new = shell_tknnew(str);
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
