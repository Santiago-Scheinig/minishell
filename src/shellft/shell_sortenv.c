/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sortenv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/02 21:09:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

/**
 * COMMENT PENDING
 */
void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		ft_printf("\"%s\"\n", envp[i++]);
}

/**
 * COMMENT PENDING
 */
void	print_export(t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;

	current = env_lst;
	while (current)
	{
		tmp = (t_var *)current->content;
		if (tmp->name && tmp->exported)
		{
			ft_printf("declare -x %s", tmp->name);
			if (tmp->value)
				ft_printf("\"%s\"", tmp->value);
			ft_printf("\n");
		}
		current = current->next;
	}
}

/**
 * COMMENT PENDING
 */
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
 * COMMENT PENDING
 */
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
			if (ft_strncmp(tmp->name, nxt->name, ft_strlen(tmp->name) + 1) > 0)
			{
				head = swap_env(head, prev, current, current->next);
				sorted = 0;
			}
			prev = current;
			current = current->next;
		}
	}
}
