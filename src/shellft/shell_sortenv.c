/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sortenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/29 15:32:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static t_list	**swap_env(t_list **head, t_list *prev, t_list *crnt, t_list *next)
{
	crnt->next = next->next;
	next->next = crnt;
	if (prev)
		prev->next = next;
	else
		*head = next;
	if (!prev)
		prev = *head;
	else
		prev = prev->next;
	return (head);
}

/**
 * Sorts a linked list of environment variables by name.
 * 
 * @param head Pointer to the pointer to the head of the list to sort.
 * 
 * Performs an in-place bubble sort over the singly-linked list by swapping
 * nodes (adjusting next pointers) to order entries ascending by name.
 * The function updates the head pointer when the first element moves.
 * 
 * @note - Modifies list links in place; it does not allocate or free nodes.
 * @note - Comparison uses ft_strncmp and assumes node content and names are
 * 		valid.
 */
void	shell_sortenv(t_list **head)
{
	t_list	*current;
	t_list	*prev;
	t_var	*nxt;
	t_var	*tmp;
	int		sorted;

	sorted = 0;
	while (!sorted)
	{
		prev = NULL;
		sorted = 1;
		current = *head;
		while (current && current->content && current->next &&current->next->content)
		{
			tmp = (t_var *)current->content;
			nxt = (t_var *)current->next->content;
			if (ft_strncmp(tmp->name, nxt->name, ft_strlen(tmp->name)) > 0)
			{
				head = swap_env(head, prev, current, current->next);
				sorted = 0;
			}
			prev = current;
			current = current->next;
		}
	}
}
