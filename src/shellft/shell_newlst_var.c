/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newlst_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:48:40 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 21:16:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Initializes a t_var structure with name and value fields.
 *
 * Sets the name field using a substring of envp up to the '=' character.
 * If '=' is present, sets the value field to the part after '='. Handles
 * memory allocation failures gracefully by freeing previously allocated 
 * memory.
 *
 * @param new	Pointer to a pre-allocated t_var structure.
 * @param envp	Pointer to the environment string ("NAME=value").
 * @param sign	Pointer to the '=' character in envp, or NULL if not found.
 * @param i		Length of the name portion (index of '=' in envp).
 *
 * @note	Returns NULL on allocation failure and frees any memory
 *			previously allocated in new.
 *
 * @return	Pointer to the initialized t_var structure, or NULL on failure.
 */
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
 * @brief Creates a t_var structure from an environment string.
 *
 * Allocates and initializes a t_var structure from a string of the form
 * "NAME=value". Sets the name and value fields and marks the variable
 * as exported or not based on the export parameter.
 *
 * @param envp		Pointer to the environment string to convert.
 * @param export	Integer flag indicating if the variable is exported (1)
 *               	or not (0).
 *
 * @note	Memory is allocated for the t_var structure and its contents.
 *			Returns NULL if allocation fails.
 *
 * @return	Pointer to the newly created t_var structure, or NULL on failure.
 */
t_var	*shell_create_var(const char *envp, int export)
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
 * @brief Creates a linked list of environment variable nodes from an array.
 *
 * Iterates over the envp array, creating a t_var structure for each
 * environment string using create_envp(), then stores each in a new
 * linked list node. Returns the head of the list.
 *
 * @param envp	Pointer to the array of environment strings.
 *
 * @note	Memory is allocated for each t_var and list node. If allocation
 *			fails, previously allocated nodes are freed using shell_lstclear().
 *
 * @return	Pointer to the head of the newly created linked list, or NULL
 *			on failure.
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
		content = shell_create_var(envp[i], 1);
		if (!content)
		{
			shell_lstclear(&head, shell_lstdel_var);
			return (NULL);
		}
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			shell_lstclear(&head, shell_lstdel_var);
			return (NULL);
		}
		ft_lstadd_back(&head, new_node);
	}
	return (head);
}
