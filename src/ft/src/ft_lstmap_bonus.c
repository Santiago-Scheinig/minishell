/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 20:51:56 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:52:16 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Creates a new list by applying a function to each element.
 *
 * 			Iterates through 'lst' and applies the function 'f' to the
 *			content of each node. Creates a new list from the results.
 *			If allocation fails, uses 'del' to free content and clears
 *			the new list.
 *
 * @param	lst	Pointer to the first node of the original list.
 * @param	f	Function to apply to the content of each node.
 * @param	del	Function used to free content in case of allocation failure.
 *
 * @note	The original list is not modified.
 * @note	If 'lst', 'f', or 'del' is NULL, returns NULL.
 *
 * @return	Pointer to the newly created list, or NULL on failure.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp_lst;
	t_list	*tmp_node;
	void	*cont;

	tmp_lst = NULL;
	if (!lst || !f || !del)
		return (tmp_lst);
	while (lst)
	{
		cont = f(lst->content);
		if (!cont)
			return (ft_lstclear(&tmp_lst, del), NULL);
		tmp_node = ft_lstnew(cont);
		if (!tmp_node)
			return (del(cont), ft_lstclear(&tmp_lst, del), NULL);
		ft_lstadd_back(&tmp_lst, tmp_node);
		lst = lst->next;
	}
	return (tmp_lst);
}
