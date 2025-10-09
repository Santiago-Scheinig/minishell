/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envchr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 04:17:41 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Retrieves the value of an environment variable from a list.
 *
 *			Searches lst_envp for a variable with the given name. Sorts the
 *			list first using shell_sortenv(). If found, stores the exported
 *			flag in the provided pointer and returns the value string.
 *
 * @param	exported	Pointer to an int to store the variable's 
 * 						exported flag.
 * @param	name		Name of the environment variable to search for.
 * @param	lst_envp	Pointer to the linked list of t_var environment 
 * 						variables.
 *
 * @note	Returns NULL if the variable is not found or if its value is empty.
 * @note	The returned pointer points to the string stored in the list; do
 *			not free it directly.
 *
 * @return	Pointer to the variable's value string, or NULL if not 
 * 			found or empty.
 */
char	*shell_envchr(int *exported, const char *name, t_list *lst_envp)
{
	t_var	*envar;
	
	shell_sortenv(&lst_envp);
	while (lst_envp)
	{
		if (!lst_envp->content)
			return (NULL);
		envar = (t_var *) lst_envp->content;
		if (!ft_strncmp(envar->name, name, (ft_strlen(name))))
		{
			if (exported)
				(*exported) = envar->exported;
			if (!envar->value[0])
				return (NULL);
			return (envar->value);
		}
		lst_envp = lst_envp->next;
	}
	return (NULL);
}
