/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:59:50 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:50:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Adds a new element at the beginning of a linked list.
 *
 * 			Inserts the 'new' node at the front of the list pointed to by
 *			'lst'. The previous first element becomes the second node.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	new	Node to add at the front of the list.
 *
 * @note	Does nothing if 'new' is NULL.
 */
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new->next = lst[0];
	lst[0] = new;
}
