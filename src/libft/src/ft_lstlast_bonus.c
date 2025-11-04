/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:26:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:51:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Returns the last node of a linked list.
 *
 * 			Traverses the list 'lst' and returns a pointer to the last node.
 *			If the list is empty, returns NULL.
 *
 * @param	lst	Pointer to the first node of the list.
 *
 * @return	Pointer to the last node of the list, or NULL if the list is
 *			empty.
 */
t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}
