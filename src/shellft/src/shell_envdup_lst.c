/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup_lst.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:48:40 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/18 20:08:57 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Duplicates an environment array into a linked list of t_var nodes.
 *
 *			Iterates through the envp array, creating a new t_var with
 *			shell_envnew() for each entry. Each t_var is wrapped in a list
 *			node and appended to the new linked list. If any allocation fails,
 *			all previously allocated nodes are freed.
 *
 * @param	envp	Pointer to the original NULL-terminated environment array.
 *
 * @note	Returns NULL if allocation fails at any point.
 * @note	The returned list and its t_var contents must be freed with
 *			shell_lstclear() and shell_delenv().
 *
 * @return	Pointer to the newly created linked list of t_var nodes, or NULL
 *			on error.
 */
t_list	*shell_envdup_lst(char **envp)
{
	t_list	*lst_dup;
	t_list	*new_node;
	t_var	*envar;
	int		i;

	i = -1;
	lst_dup = NULL;
	while (envp && envp[++i])
	{
		envar = shell_newenv(envp[i], 1);
		if (!envar && i)
		{
			ft_lstclear(&lst_dup, shell_delenvar);
			return (NULL);
		}
		new_node = ft_lstnew(envar);
		if (!new_node)
		{
			free(envar);
			ft_lstclear(&lst_dup, shell_delenvar);
			return (NULL);
		}
		ft_lstadd_back(&lst_dup, new_node);
	}
	return (lst_dup);
}
