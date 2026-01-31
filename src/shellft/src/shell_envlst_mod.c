/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envlst_mod.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:21:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/12/15 15:20:30 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

static int	swap_value(int e, char *var, t_var *envar, char ***envp)
{
	if (get_value(var, envar))
		return (MSHELL_FAILURE);
	envar->exported = e;
	if (e)
		if (shell_envarr_swp(var, envp))
			return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Adds a new environment variable to a linked list and optionally
 * 			envp.
 *
 *			Creates a new t_var from var and appends it to the linked list.
 *			If exp is true, also adds the variable to the envp array. Frees
 *			memory on allocation failure.
 *
 * @param	exp			Flag indicating whether the variable should be
 * 						exported.
 * @param	var			String representing the new environment variable
 * 						(NAME=VALUE).
 * @param	envp		Double pointer to the environment array for exported
 * 						variables.
 * @param	lst_envp	Pointer to the linked list of environment variables.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for managing the linked list and envp.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int	shell_envlst_add(int e, char *var, char ***envp, t_list *lst_envp)
{
	t_var	*envar;
	t_list	*new_node;

	envar = shell_newenv(var, e);
	if (!envar)
		return (MSHELL_FAILURE);
	new_node = ft_lstnew(envar);
	if (!new_node)
	{
		free(envar);
		return (MSHELL_FAILURE);
	}
	ft_lstadd_back(&lst_envp, new_node);
	if (e)
		if (shell_envarr_add(var, envp))
			return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Updates the value and exported flag of an environment variable.
 *
 *			Searches the linked list of t_var for a variable matching the
 *			name in var. If found, swaps its value and updates the exported
 *			flag. If exported is true, adds the variable to the envp array.
 *
 * @param	e			New exported flag to set for the variable.
 * @param	var			String containing the variable name and new value
 * 						(NAME=VALUE).
 * @param	envp		Double pointer to the environment array for addition
 * 						if exported.
 * @param	lst_envp	Pointer to the linked list of environment variables.
 *
 * @note	Returns MSHELL_FAILURE if var is NULL, memory allocation fails,
 *			or swap_value fails.
 * @note	The caller is responsible for managing the envp array and linked
 * 			list.
 *
 * @return	MSHELL_SUCCESS on successful update, MSHELL_FAILURE otherwise.
 */
int	shell_envlst_swp(int e, char *var, char ***envp, t_list *lst_envp)
{
	t_var	*envar;
	char	*var_name;

	if (!var)
		return (MSHELL_FAILURE);
	var_name = get_name(var);
	if (!var_name)
		return (MSHELL_FAILURE);
	while (lst_envp)
	{
		envar = (t_var *) lst_envp->content;
		if (!envar)
			return (MSHELL_FAILURE);
		if (!ft_strncmp(var_name, envar->name, ft_strlen(envar->name) + 1))
		{
			if (swap_value(e, var, envar, envp))
				return (MSHELL_FAILURE);
			break ;
		}
		lst_envp = lst_envp->next;
	}
	free(var_name);
	return (MSHELL_SUCCESS);
}
