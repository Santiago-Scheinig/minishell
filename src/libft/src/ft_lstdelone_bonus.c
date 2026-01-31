/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:54:33 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:51:02 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Deletes a single node from a linked list.
 *
 * 			Applies the 'del' function to the node's content, then frees
 *			the node itself. Does not affect other nodes in the list.
 *
 * @param	lst	Node to delete.
 * @param	del	Function used to free the content of the node.
 *
 * @note	If 'lst->content' is NULL, only the node is freed.
 */
void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst->content != NULL)
		del(lst->content);
	free(lst);
}
