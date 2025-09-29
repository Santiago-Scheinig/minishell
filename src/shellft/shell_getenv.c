/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/27 15:14:54 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * Retrieves the value of an environment variable from the variable list.
 * 
 * @param lst_var Pointer to the linked list of environment variables
 * 		(t_var nodes).
 * @param name Name of the environment variable to find
 * 		(null-terminated string).
 * 
 * Searches the sorted variable list for a node whose name matches [name].
 * If found and the variable has a non-empty value, returns a pointer to that
 * value.
 * The returned pointer refers to the string owned by the list node.
 * 
 * @return Pointer to the value string, or NULL if the variable is not found
 *         or its value is empty.
 * @note - The function sorts a temporary copy of the list pointer before
 * 			searching.
 * @note - Caller must not modify or free the returned pointer.
 */
char	*shell_getenv(t_list *lst_var, const char *name)
{
	t_list	*tmp;
	t_var	*aux;
		
	tmp = lst_var;
	shell_sortenv(&tmp);
	while (tmp && tmp->content)
	{
		aux = (t_var *)tmp->content;
		if (!ft_strncmp(aux->name, name, (ft_strlen(name))))
		{
			if (!aux->value[0])
				return (NULL);
			return (aux->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
