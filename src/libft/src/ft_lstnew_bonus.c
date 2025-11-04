/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:49:54 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:52:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Creates a new node for a linked list.
 *
 * 			Allocates memory for a new list node, assigns 'content' to it,
 *			and sets the 'next' pointer to NULL.
 *
 * @param	content	Pointer to the content to store in the new node.
 *
 * @return	Pointer to the newly created node, or NULL if allocation fails.
 */
t_list	*ft_lstnew(void *content)
{
	t_list	*lst;

	lst = malloc(sizeof(t_list));
	if (!lst)
		return (NULL);
	lst->content = content;
	lst->next = NULL;
	return (lst);
}
