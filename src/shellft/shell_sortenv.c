/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sortenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/07 15:31:51 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

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

void	sortenv(t_list **head)
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
		while (current && current->next)
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
