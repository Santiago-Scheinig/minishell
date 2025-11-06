/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstadd_newtkn.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 09:53:31 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

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
 * REWRITE
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
