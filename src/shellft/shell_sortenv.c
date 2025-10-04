/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sortenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 22:18:37 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief	Swaps two consecutive nodes in a linked list of 
 * 			environment variables.
 *
 * Adjusts pointers to swap the middle node (mid) with its next node (nxt).
 * Updates the previous node (prv) or the list head (*var) as needed.
 *
 * @param var  Pointer to the head of the linked list.
 * @param prv  Previous node before mid, or NULL if mid is the first node.
 * @param mid  The node to swap with its next node.
 * @param nxt  The node following mid to be swapped.
 *
 * @note  	Returns the head pointer of the updated list. Useful for sorting.
 *
 * @return 	Pointer to the (possibly new) head of the list.
 */
static t_list	**swap_env(t_list **var, t_list *prv, t_list *mid, t_list *nxt)
{
	mid->next = nxt->next;
	nxt->next = mid;
	if (prv)
		prv->next = nxt;
	else
		*var = nxt;
	if (!prv)
		prv = *var;
	else
		prv = prv->next;
	return (var);
}

/**
 * @brief Sorts a linked list of environment variables alphabetically by name.
 *
 * Performs a simple bubble sort on the linked list of t_var structures
 * pointed to by head. Uses swap_env() to reorder adjacent nodes when needed.
 *
 * @param var 	Pointer to the head of the linked list of 
 * 				environment variables.
 *
 * @note  Sorting is done in-place; no new nodes are allocated.
 * @note  Each node's content must be cast to t_var to access name strings.
 */
void	shell_sortenv(t_list **var)
{
	t_list	*mid;
	t_list	*prv;
	t_var	*nxt;
	t_var	*tmp;
	int		sorted;

	sorted = 0;
	while (!sorted)
	{
		prv = NULL;
		sorted = 1;
		mid = *var;
		while (mid && mid->content && mid->next && mid->next->content)
		{
			tmp = (t_var *)mid->content;
			nxt = (t_var *)mid->next->content;
			if (ft_strncmp(tmp->name, nxt->name, ft_strlen(tmp->name)) > 0)
			{
				var = swap_env(var, prv, mid, mid->next);
				sorted = 0;
			}
			prv = mid;
			mid = mid->next;
		}
	}
}
