/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envarr_mod.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 03:18:38 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 04:43:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

static int	swap_value(char	**envp, char *var)
{
	char	*tmp;

	tmp = (*envp);
	(*envp) = ft_strdup(var);
	if (!(*envp))
	{
		(*envp) = tmp;
		return (MSHELL_FAILURE);
	}
	free(tmp);
	return (MSHELL_SIG_HANDLR);
}

/**
 * @brief	Replaces the value of an existing environment variable.
 *
 *			Searches the envp array for a variable with the same name as var.
 *			If found, swaps its value with the new one. Memory for temporary
 *			names is allocated and freed internally.
 *
 * @param	var		Pointer to the string containing the new variable value.
 * @param	envp	Double pointer to the environment array to modify.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for managing the envp array.
 *
 * @return	MSHELL_SUCCESS on successful swap, MSHELL_FAILURE otherwise.
 */
int	shell_envarr_swp(char *var, char ***envp)
{
	char	*envp_name;
	char	*var_name;
	int		i;

	var_name = get_name(var);
	if (!var_name)
		return (1);
	i = -1;
	while ((*envp)[++i])
	{
		envp_name = get_name((*envp)[i]);
		if (!envp_name)
			return (MSHELL_FAILURE);
		if (!ft_strncmp(var_name, envp_name, ft_strlen(var_name) + 1))
		{
			if (swap_value((*envp)[i]), var)
				return (MSHELL_FAILURE);
			break;
		}
		free(envp_name);
	}
	free(envp_name);
	free(var_name);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Adds a new environment variable to the envp array.
 *
 *			Reallocates the envp array to hold one additional variable,
 *			copies the new variable string, and ensures the array remains
 *			NULL-terminated.
 *
 * @param	var		Pointer to the string representing the environment variable
 *			to add.
 * @param	envp	Double pointer to the environment array to modify.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for freeing the envp array and its
 *			strings.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int	shell_envarr_add(char *var, char ***envp)
{
	int	envp_len;
	int	i;

	i = -1;
	envp_len = sizeof(char *) * (ft_arrlen((const void **) (*envp)) + 1);
	(*envp) = ft_realloc((void *) (*envp), envp_len + sizeof(char *), envp_len);
	if (!(*envp))
		return (MSHELL_FAILURE);
	envp_len = ft_arrlen((const void **) (*envp));
	(*envp)[envp_len] = ft_strdup(var);
	if (!(*envp)[envp_len])
		return (MSHELL_FAILURE);
	(*envp)[envp_len + 1] = NULL;
	return (MSHELL_SUCCESS);
}