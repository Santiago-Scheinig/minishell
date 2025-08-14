/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstclear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:32:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/14 21:34:39 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * Frees the content of a NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
static void	shell_lstdelone(t_list *lst, void (*del)(void *))
{
	t_token *word;

	if (lst->content != NULL)
	{
		word = (t_token *) lst->content;
		if (word->str)
			del(word->str);
		del(lst->content);
	}
	free(lst);
}

/**
 * Frees every node included on the LIST HEAD. 
 * @param lst The LIST HEAD to free.
 * @param del The function used to free each node.
 * @note In general, the del function should be free().
 */
void	shell_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	while ((*lst))
	{
		tmp = (*lst)->next;
		shell_lstdelone((*lst), del);
		(*lst) = tmp;
	}
}