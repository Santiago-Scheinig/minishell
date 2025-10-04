/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 20:36:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Increments a numeric value stored in an environment string.
 *
 * Assumes envp points to a string of the form "NAME=value" where the
 * numeric value starts at index 6. Duplicates the substring, converts
 * it to an integer, increments it by 1, and returns the result.
 *
 * @param envp	Pointer to the environment string to process.
 *
 * @note	Memory is allocated for a temporary copy of the value; it is
 *			freed before returning.
 * @note	Returns -1 if memory allocation fails.
 *
 * @return	The incremented integer value on success, -1 on allocation failure.
 */
static int	change_value(const char *envp)
{
	int		value;
	char	*tmp;

	tmp = ft_strdup(&envp[6]);
	if (!tmp)
		return (-1);
	value = ft_atoi(tmp) + 1;
	free(tmp);
	return (value);
}

/**
 * @brief Checks if an environment string contains a valid numeric value.
 *
 * Assumes the string starts with a prefix of 6 characters (e.g., "NAME=")
 * and checks that all subsequent characters are digits. Ensures that
 * the string does not end with '=' alone.
 *
 * @param str	Pointer to the string to validate.
 *
 * @note	Used to validate numeric environment variables like SHLVL.
 *
 * @return	1 if the string is a valid numeric value, 0 otherwise.
 */
static size_t	is_valid(const char *str)
{
	size_t	i;

	i = 6;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (i == ft_strlen(str) && str[i - 1] != '=')
		return (1);
	return (0);
}

/**
 * @brief Checks and updates the SHLVL environment variable.
 *
 * If envp starts with "SHLVL=", verifies whether its value is a valid
 * number. If valid, increments the value by 1 using change_value().
 * If invalid, sets the value to 1. Constructs a new string "SHLVL=value"
 * and returns it.
 *
 * @param envp	Pointer to the environment string to check.
 *
 * @note	Memory is allocated for the returned string; caller is
 *			responsible for freeing it.
 * @note	Returns NULL on memory allocation failure.
 * @note	Only processes strings starting with "SHLVL="; other strings
 *			return NULL.
 *
 * @return	Pointer to the new SHLVL string on success, NULL on error
 *			or if not "SHLVL=".
 */
static char	*check_value(const char *envp)
{
	int		value;
	char	*numb;
	char	*aux;
	char	*tmp;

	if (!ft_strncmp(envp, "SHLVL=", 6))
	{
		if (is_valid(envp))
		{
			value = change_value(envp);
			if (value == -1)
				return (NULL);
		}
		else
			value = 1;
		tmp = ft_strdup("SHLVL=");
		numb = ft_itoa(value);
		aux = ft_strjoin(tmp, numb);
		free(tmp);
		free(numb);
		if (!aux)
			return (NULL);
		return (aux);
	}
	return (NULL);
}

/**
 * @brief Duplicates the environment variable array with SHLVL adjustment.
 *
 * Allocates a new array of strings, duplicating each entry from envp.
 * If an entry starts with "SHLVL=", it is processed by check_value()
 * to increment or reset the shell level. Other entries are duplicated
 * normally.
 *
 * @param envp	Pointer to the original environment array.
 *
 * @note	Memory is allocated for the array and each string; the caller
 *			is responsible for freeing all of it.
 * @note	Returns NULL if memory allocation fails at any point.
 *
 * @return	Pointer to the newly allocated environment array, or NULL
 *			on allocation failure.
 */
char	**shell_envpdup(const char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = ft_arrlen((const void **)envp);
	tmp = malloc((i + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		tmp[j] = check_value(envp[j]);
		if (!tmp[j])
		{
			tmp[j] = ft_strdup(envp[j]);
			if (!tmp[j])
			{
				while (--j >= 0)
					free(tmp[j]);
				free(tmp);
				return (NULL);
			}
		}
	}
	tmp[i] = NULL;
	return (tmp);
}
