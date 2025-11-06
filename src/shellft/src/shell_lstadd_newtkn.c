/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstadd_newtkn.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:48:10 by sscheini         ###   ########.fr       */
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
 * REWRITE
 */
int	shell_lstadd_newtkn(int i, char *str, char *base, t_list *lst_tkn)
{
	t_token	*tkn_new;
	t_list	*new_node;

	if (!i)
	{
		rewrite_node(str, ((t_token *) lst_tkn->content));
		return (MSHELL_SUCCESS);
	}
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
