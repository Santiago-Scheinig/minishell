/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 01:46:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 02:48:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Creates and initializes a new t_var structure from env data.
 *
 *			Allocates a t_var element, extracts the variable name from envp
 *			up to index i, and optionally duplicates its value if provided.
 *			If any allocation fails, frees all allocated memory and returns
 *			NULL.
 *
 * @param	envp	Pointer to the environment string (e.g., "PATH=/usr/bin").
 * @param	value	Pointer to the '=' character in envp or NULL if absent.
 * @param	i		Index position where the variable name ends.
 *
 * @note	If value is NULL, the variable will be created with no value.
 * @note	Returned structure must be freed with shell_delenv() when done.
 *
 * @return	Pointer to the newly allocated t_var structure, or NULL on error.
 */
static t_var	*set_envar(const char *envp, char *value, int i)
{
	t_var	*envar;

	envar = ft_calloc(sizeof(t_var));
	if (!envar)
		return (NULL);
	envar->name = ft_substr(envp, 0, i);
	if (!envar->name)
	{
		free(envar);
		return (NULL);
	}
	if (!value)
		return (envar);
	envar->value = ft_strdup(value + 1);
	if (!envar->value)
	{
		shell_delenv(envar);
		return (NULL);
	}
	return (envar);
}

/**
 * @brief	Creates a new t_var environment variable from a raw string.
 *
 *			Parses the given envp string to separate the variable name and
 *			value, then calls set_envar() to allocate and initialize a new
 *			t_var structure. The exported flag is assigned accordingly.
 *
 * @param	envp		Pointer to the environment string (e.g., "USER=root").
 * @param	exported	Boolean flag indicating if the variable is exported.
 *
 * @note	Returns NULL if allocation fails or envp is invalid.
 * @note	The returned structure must be freed with shell_delenv() when
 *			no longer needed.
 *
 * @return	Pointer to the newly created t_var structure, or NULL on error.
 */
t_var	*shell_newenv(const char *envp, int exported)
{
	t_var	*envar_new;
	char	*value;
	int		i;

	i = 0;
	value = ft_strchr(envp, '=');
	while (envp[i] && envp[i] != '=')
		i++;
	envar_new = set_envar(envp, value, i);
	if (!envar_new)
		return (NULL);
	envar_new->exported = exported;
	return (envar_new);
}