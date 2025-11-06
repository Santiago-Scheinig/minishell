/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:03:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:50:38 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Clears a linked list and frees its memory.
 *
 * 			Iterates through the list pointed to by 'lst', applies the
 *			'del' function to free the content of each node, and frees
 *			each node. Sets '*lst' to NULL after clearing.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	del	Function used to free the content of each node.
 *
 * @note	After this function, the list pointer will be NULL.
 */
void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	while ((*lst))
	{
		tmp = (*lst)->next;
		ft_lstdelone((*lst), del);
		(*lst) = tmp;
	}
}
