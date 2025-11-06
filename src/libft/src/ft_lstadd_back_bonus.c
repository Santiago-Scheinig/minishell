/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:36:37 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:49:53 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Adds a new element at the end of a linked list.
 *
 * 			Appends the 'new' node at the end of the list pointed to by
 *			'lst'. If the list is empty, 'new' becomes the first element.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	new	Node to add at the end of the list.
 *
 * @note	Does nothing if 'new' is NULL.
 */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	tmp = ft_lstlast(*lst);
	if (!tmp)
	{
		*lst = new;
		return ;
	}
	tmp->next = new;
}
