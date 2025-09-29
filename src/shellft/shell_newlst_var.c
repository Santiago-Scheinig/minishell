/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newlst_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:48:40 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/27 15:07:14 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static t_var	*init_envp(t_var *new, const char *envp, char *sign, int i)
{
	new->name = ft_substr(envp, 0, i);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (!sign)
	{
		new->value = NULL;
		return (new);
	}
	new->value = ft_strdup(sign + 1);
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	return (new);
}

/**
 * Creates a new t_var node from an environment string.
 *
 * @param envp Environment string in the form "NAME" or "NAME=VALUE".
 * @param export Integer flag indicating whether the variable is exported (1)
 * 		or not (0).
 *
 * Allocates a t_var structure and fills its name and value by parsing envp.
 * Sets the exported flag according to the provided parameter.
 *
 * @return Pointer to the newly created t_var on success, or NULL on allocation
 * 		failure.
 * @note Returned structure must be freed by the caller
 * 		(use shell_lstdelvar or equivalent).
 */
t_var	*create_envp(const char *envp, int export)
{
	t_var	*new;
	char	*sign;
	int		i;

	i = 0;
	sign = ft_strchr(envp, '=');
	new = malloc(sizeof(t_var));
	if (!new)
		return (NULL);
	while (envp[i] && envp[i] != '=')
		i++;
	new = init_envp(new, envp, sign, i);
	if (!new)
		return (NULL);
	new->exported = export;
	return (new);
}

/**
 * Builds a linked list of t_var nodes from a null-terminated envp array.
 *
 * @param envp Null-terminated array of environment strings.
 *
 * Iterates envp, creates a t_var for each entry and appends it to a new list.
 * On allocation failure the function frees any previously allocated nodes and
 * returns NULL.
 *
 * @return Pointer to the head of the newly created list on success, or NULL on
 * 		failure.
 * @note Caller owns the returned list and must free it with shell_lstdelvar.
 */
t_list	*shell_newlst_var(char **envp)
{
	t_var	*content;
	t_list	*new_node;
	t_list	*head;
	int		i;

	new_node = NULL;
	head = NULL;
	i = -1;
	while (envp && envp[++i])
	{
		content = create_envp(envp[i], 1);
		if (!content)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return (NULL);
		}
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return (NULL);
		}
		ft_lstadd_back(&head, new_node);
	}
	return (head);
}
