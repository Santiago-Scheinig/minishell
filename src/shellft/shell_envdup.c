/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 02:22:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

/**
 * @brief	Increments the SHLVL environment variable value.
 *
 *			Extracts the numeric part from the provided SHLVL string, 
 *			increments it by one, and returns a newly allocated string 
 *			containing "SHLVL=" followed by the new value.
 *
 * @param	shlvl	Pointer to the original SHLVL string (e.g., "SHLVL=2").
 *
 * @note	If the numeric part is missing or invalid, initializes SHLVL to 1.
 * @note	Returned string must be freed by the caller.
 * @note	Returns NULL if memory allocation fails at any step.
 *
 * @return	Newly allocated string with updated SHLVL value, or NULL on error.
 */
static char	*change_shlvl(const char *shlvl)
{
	char	*shlvl_dup;
	char	*tmp_name;
	char	*tmp_value;
	int		lvl;

	shlvl = ft_strchr(shlvl, "=");
	if (!shlvl[1] || !ft_isdigit(shlvl[1]))
		lvl = 1;
	else
		lvl = ft_atoi(&shlvl[1]) + 1;
	tmp_name = ft_strdup("SHLVL=");
	if (!tmp_name)
		return (NULL);
	tmp_value = ft_itoa(lvl);
	if (!tmp_value)
	{
		free(tmp_name);	
		return (NULL);
	}
	shlvl_dup = ft_strjoin(tmp_name, tmp_value);
	free(tmp_name);
	free(tmp_value);
	if (!shlvl_dup)
		return (NULL);
	return (shlvl_dup);
}

/**
 * @brief	Duplicates the environment variable array and updates SHLVL.
 *
 *			Allocates a new NULL-terminated copy of the given envp array.
 *			Each string is duplicated with ft_strdup(). When "SHLVL=" is
 *			found, its value is incremented using change_shlvl(). If any
 *			allocation fails, all previously allocated memory is freed.
 *
 * @param	envp	Pointer to the original environment variable array.
 *
 * @note	Returns NULL if allocation fails or if envp is invalid.
 * @note	The returned array and its contents must be freed by the caller.
 *
 * @return	Newly allocated environment array with updated SHLVL, or NULL
 *			on allocation failure.
 */
char	**shell_envdup(const char **envp)
{
	char		**envp_dup;
	int			i;
	int			j;

	i = ft_arrlen((const void **) envp);
	envp_dup = ft_calloc((i + 1), sizeof(char *));
	if (!envp_dup)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		envp_dup[j] = ft_strdup(envp[j]);
		if (!envp_dup[j])
			return (memfree(envp_dup, j));
		if (!ft_strncmp(envp_dup[j], "SHLVL=", 6))
		{
			envp_dup[j] = change_shlvl(envp_dup[j]);
			if (!envp_dup[j])
				return (memfree(envp_dup, j));
		}
	}
	return (envp_dup);
}
