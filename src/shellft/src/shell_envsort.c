/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envsort.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/18 17:57:50 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

static t_list	**swap_env(t_list **lst, t_list *prv, t_list *mid, t_list *nxt)
{
	mid->next = nxt->next;
	nxt->next = mid;
	if (prv)
		prv->next = nxt;
	else
		*lst = nxt;
	if (!prv)
		prv = *lst;
	else
		prv = prv->next;
	return (lst);
}

/**
 * @brief	Sorts a linked list of environment variables alphabetically.
 *
 *			Iteratively compares adjacent t_var elements in the list and
 *			swaps them using swap_env() if their names are out of order.
 *			Sorting continues until a full pass is made with no swaps,
 *			ensuring all variables are ordered lexicographically.
 *
 * @param	hlst_envp	Pointer to the head of the t_list environment list.
 *
 * @note	Uses ft_strncmp() for name comparison based on string length.
 * @note	Sorting is done in place; no new list is allocated.
 * @note	Requires that each node's content is a valid t_var structure.
 */
void	shell_envsort(t_list **hlst_envp)
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
		mid = *hlst_envp;
		while (mid && mid->content && mid->next && mid->next->content)
		{
			tmp = (t_var *)mid->content;
			nxt = (t_var *)mid->next->content;
			if (ft_strncmp(tmp->name, nxt->name, ft_strlen(tmp->name)) > 0)
			{
				hlst_envp = swap_env(hlst_envp, prv, mid, mid->next);
				sorted = 0;
			}
			prv = mid;
			mid = mid->next;
		}
	}
}
