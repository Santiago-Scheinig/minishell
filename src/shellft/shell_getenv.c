/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 20:50:34 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Retrieves the value of an environment variable from a list.
 *
 * Searches lst_var for a variable with the given name. Sorts the list
 * first using shell_sortenv(). Returns the value string if found and
 * non-empty, otherwise returns NULL.
 *
 * @param lst_var Pointer to the linked list of t_var environment variables.
 * @param name Name of the environment variable to search for.
 *
 * @note	Returns NULL if the variable is not found or if its value is empty.
 * @note	The returned pointer points to the string stored in the list;
 *			do not free it directly.
 *
 * @return	Pointer to the variable's value string, or NULL if not found 
 * 			or empty.
 */
char	*shell_getenv(t_list *lst_t_var, const char *name)
{
	t_list	*tmp;
	t_var	*aux;
	
	tmp = lst_t_var;
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
