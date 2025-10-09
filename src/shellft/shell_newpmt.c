/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newpmt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 03:05:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

/**
 * @brief	Generates the final shell prompt string using PS1, user, and path.
 *
 *			Retrieves the USER environment variable. Calculates the required
 *			length for the prompt with prompt_len(). Allocates memory and
 *			calls transform_format() to fill the string according to PS1 and
 *			path. Frees the temporary path string before returning.
 *
 * @param	ps1			Pointer to the PS1 prompt format string.
 * @param	path		Pointer to the path string to include in the prompt.
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 *
 * @note	Returns NULL if memory allocation fails.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Pointer to the newly allocated formatted prompt string, or NULL
 *			on error.
 */
static char	*pwd_name(char *ps1, char *path, t_list *lst_envp)
{
	size_t	len;
	char	*user;
	char	*tmp;

	user = shell_getenv(lst_envp, "USER");
	if (!user)
		user = NULL;
	len = prompt_len(ps1, user, path);
	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	tmp = transform_format(tmp, ps1, user, path);
	free(path);
	return (tmp);
}

/**
 * @brief	Replaces the HOME prefix in a path string with '~'.
 *
 *			Checks if the path starts with the given home directory. If so,
 *			allocates a new string with '~' followed by the remaining path.
 *			Frees the original path and returns the new shortened string.
 *
 * @param	home	Pointer to the HOME directory string.
 * @param	path	Pointer to the path string to shorten.
 *
 * @note	Returns the original path if it does not start with home.
 * @note	The returned string must be freed by the caller if newly allocated.
 *
 * @return	Pointer to the new shortened path string, or the original path
 *			if no shortening is needed.
 */
static char	*short_home(char *home, char *path)
{
	size_t	rest;
	char	*short_path;

	short_path = NULL;
	if (ft_strncmp(path, home, ft_strlen(home)) == 0)
	{
		rest = ft_strlen(path + ft_strlen(home));
		short_path = malloc(2 + rest + 1);
		if (!short_path)
		{
			free(path);
			return (NULL);
		}
		short_path[0] = '~';
		ft_strlcpy(short_path + 1, path + ft_strlen(home), rest + 1);
		free(path);
		return (short_path);
	}
	return (path);
}

/**
 * @brief	Shortens a given path by replacing the HOME directory with '~'.
 *
 *			Retrieves the HOME environment variable. If the path starts
 *			with HOME and the directory is accessible, calls short_home()
 *			to replace the home portion with '~'. Otherwise, returns the
 *			original path.
 *
 * @param	path		Pointer to the absolute path string to shorten.
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 *
 * @note	Returns the original path if HOME is not set or not accessible.
 * @note	The returned string may be newly allocated; free it if necessary.
 *
 * @return	Pointer to the shortened path string, or NULL on allocation failure.
 */
static char	*short_path_name(char *path, t_list *lst_envp)
{
	char	*home;
	int		i;

	home = shell_getenv(lst_envp, "HOME");
	if (!home)
		return (path);
	i = ft_strlen(home);
	if (home && home[i - 1] && home[i - 1] != '/' && !access(home, X_OK))
		path = short_home(home, path);
	if (!path)
		return (NULL);
	return (path);
}

/**
 * @brief	Gets the current working directory and formats it for the prompt.
 *
 *			Uses getcwd() to obtain the absolute path. If getcwd() fails,
 *			falls back to the PWD environment variable. Calls short_path_name()
 *			to generate a possibly shortened version of the path for display.
 *
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 *
 * @note	Returns NULL if memory allocation fails or if path retrieval fails.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Pointer to the newly allocated (possibly shortened) path string,
 *			or NULL on error.
 */
static char	*path_cwd(t_list *lst_envp)
{
	char	*prompt;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		path = ft_strdup(shell_getenv(lst_envp, "PWD"));
	if (!path)
		return (NULL);
	prompt = short_path_name(path, lst_envp);
	if (!prompt)
		return (NULL);
	return (prompt);
}

/**
 * @brief	Generates the shell prompt string based on PS1 and current path.
 *
 *			Retrieves the PS1 environment variable and the current working
 *			directory. Calls pwd_name() to construct the final prompt string.
 *			If PS1 is not set, uses an empty string as default. Allocates a
 *			new string which must be freed by the caller.
 *
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 *
 * @note	Returns NULL if memory allocation fails at any stage.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Pointer to the newly allocated prompt string, or NULL on error.
 */
char	*shell_newpmt(t_list *lst_envp)
{
	char	*prompt;
	char	*path;
	char	*ps1;

	ps1 = shell_getenv(lst_envp, "PS1");
	path = path_cwd(lst_envp);
	if (!ps1)
	{
		ps1 = ft_strdup("");
		if (!ps1)
			return (NULL);
		prompt = pwd_name(ps1, path, lst_envp);
		free(ps1);
	}
	else
		prompt = pwd_name(ps1, path, lst_envp);
	if (!prompt)
	{
		free(path);
		return (NULL);
	}
	return (prompt);
}
