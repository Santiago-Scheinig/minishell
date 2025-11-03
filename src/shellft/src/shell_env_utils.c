/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_env_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 04:38:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:43:36 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Creates a string representing the current working directory.
 *
 * 			Calls getcwd() to retrieve the current working directory and
 * 			prefixes it with "PWD=" to form a standard environment variable
 * 			string.
 *
 * @note	The returned string must be freed by the caller to avoid memory
 * 			leaks.
 *
 * @return	Pointer to a newly allocated string in the format "PWD=/path/to/dir",
 * 			or NULL if getcwd() fails or memory allocation fails.
 */
char	*pwdstr(void)
{
	char	*tmp;
	char	*pwd;

	tmp = getcwd(NULL, 0);
	if (!tmp)
		return (NULL);
	pwd = ft_strjoin("PWD=", tmp);
	free(tmp);
	if (!pwd)
		return (NULL);
	return (pwd);
}

/**
 * @brief	Extracts the name part of an environment variable string.
 *
 *			Scans the input string until the first '=' character or the end
 *			of the string, and duplicates that portion as the variable name.
 *
 * @param	var	String containing the variable in the form "NAME=VALUE"
 *				or simply "NAME".
 *
 * @note	The returned string is dynamically allocated and must be freed
 *			by the caller. If memory allocation fails, returns NULL.
 *
 * @return	Pointer to the duplicated variable name, or NULL on failure.
 */
char	*get_name(char *var)
{
	char	*name;
	int		i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	name = ft_substr(var, 0, i);
	if (!name)
		return (NULL);
	name[i] = '\0';
	return (name);
}

/**
 * @brief	Extracts the value part of an environment variable string.
 *
 *			Searches for the '=' character in the given variable string.
 *			If found, duplicates the portion after it as the variable value.
 *			If '=' is missing, assigns NULL to the output pointer.
 *
 * @param	var		String containing the variable in the form "NAME=VALUE".
 * @param	value	Double pointer where the duplicated value string is stored.
 *
 * @note	If the variable has no value (e.g., "NAME="), an empty string is
 *			duplicated instead of NULL. Caller is responsible for freeing it.
 *
 * @return	MSHELL_SUCCESS on success, or MSHELL_FAILURE if memory allocation
 *			for the duplicated value fails.
 */
int	get_value(char *var, char **value)
{
	if (!ft_strchr(var, '='))
		(*value) = NULL;
	else
	{
		var = ft_strchr(var, '=');
		if (!var[1])
			(*value) = ft_strdup("");
		else
			(*value) = ft_strdup(++var);
		if (!(*value))
			return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}
