/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:16:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:51:31 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Applies a function to each element of a linked list.
 *
 * 			Iterates through the list 'lst' and applies the function 'f'
 *			to the content of each node.
 *
 * @param	lst	Pointer to the first node of the list.
 * @param	f	Function to apply to the content of each node.
 *
 * @note	Does nothing if 'lst' is NULL.
 */
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst)
		return ;
	f(lst->content);
	ft_lstiter(lst->next, f);
}
