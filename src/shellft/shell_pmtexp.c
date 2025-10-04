/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 22:17:53 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"
#include "lib/msh_std_utils.h"

/**
 * @brief Generates the final shell prompt string with username and path.
 *
 * Retrieves the USER environment variable, calculates the display length
 * of the prompt using prompt_len(), allocates memory for the full prompt,
 * and fills it using transform_format(). Frees the original path string
 * after use.
 *
 * @param ps1		Pointer to the prompt format string (PS1).
 * @param path		Pointer to the current working directory string.
 * @param lst_t_var	Pointer to the environment variable linked list
 * 					(t_list of t_var).
 *
 * @note	Memory is allocated for the resulting prompt string. Caller
 *			is responsible for freeing it.
 * @note	If USER is not set, username is treated as NULL.
 *
 * @return	Pointer to the dynamically allocated prompt string, or NULL
 *			on allocation failure.
 */
static char	*pwd_name(char *ps1, char *path, t_list *lst_t_var)
{
	size_t	len;
	char	*user;
	char	*tmp;

	user = shell_getenv(lst_t_var, "USER");
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
 * @brief Replaces the HOME prefix in a path with '~'.
 *
 * If the given path starts with the home directory string, allocates a new
 * string where the HOME portion is replaced by '~'. Frees the original path
 * if a new string is created.
 *
 * @param home 	Pointer to the HOME directory string.
 * @param path 	Pointer to the current path string. May be freed if replaced.
 *
 * @note	Memory is allocated for the new string. Caller is responsible
 *			for freeing the returned string.
 * @note	If path does not start with home, returns the original path.
 *
 * @return 	Pointer to the modified path string with '~', or the original
 *			path if no change is needed, or NULL on allocation failure.
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
 * @brief Shortens the current path by replacing the home directory with '~'.
 *
 * Compares the given path with the HOME environment variable. If the path
 * starts with HOME and HOME is accessible, replaces the matching prefix
 * with '~' using short_home().
 *
 * @param path 			Pointer to the current working directory string.
 * @param lst_t_var  	Pointer to the environment variable linked list 
 * 						(t_list of t_var).
 *
 * @note	Memory may be reallocated by short_home(). Caller is responsible
 *			for freeing the returned string if modified.
 * @note	Returns the original path if HOME is not set or not accessible.
 *
 * @return 	Pointer to the potentially shortened path string,
 * 			or NULL on failure.
 */
static char	*short_path_name(char *path, t_list *lst_t_var)
{
	char	*home;
	int		i;

	home = shell_getenv(lst_t_var, "HOME");
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
 * @brief Retrieves and formats the current working directory for the prompt.
 *
 * Attempts to get the current working directory using getcwd(). If it fails,
 * falls back to the PWD environment variable. The resulting path is then
 * processed with short_path_name() to potentially shorten or format 
 * it for display.
 *
 * @param lst_t_var 	Pointer to the environment variable linked list 
 * 						(t_list of t_var).
 *
 * @note	Memory is allocated for the returned string and must be freed
 *			by the caller.
 * @note	Returns NULL if both getcwd() and PWD are unavailable or on
 *			memory allocation failure.
 *
 * @return 	Pointer to the formatted current working directory string,
 *			or NULL on failure.
 */
static char	*path_cwd(t_list *lst_t_var)
{
	char	*prompt;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		path = ft_strdup(shell_getenv(lst_t_var, "PWD"));
	if (!path)
		return (NULL);
	prompt = short_path_name(path, lst_t_var);
	if (!prompt)
		return (NULL);
	return (prompt);
}

/**
 * @brief Expands the PS1 shell prompt using environment variables.
 *
 * Retrieves the PS1 variable from the environment list. If not set,
 * uses an empty string. Resolves the current working directory using
 * path_cwd() and generates the final prompt string via pwd_name().
 *
 * @param lst_t_var 	Pointer to the environment variable linked list 
 * 						(t_list of t_var).
 *
 * @note	Memory is allocated for the resulting prompt string and must
 *			be freed by the caller.
 * @note	If PS1 is not set, an empty string is used as default.
 *
 * @return	Pointer to the dynamically allocated expanded prompt string,
 *			or NULL on failure.
 */
char	*shell_pmtexp(t_list *lst_t_var)
{
	char	*prompt;
	char	*path;
	char	*ps1;

	ps1 = shell_getenv(lst_t_var, "PS1");
	path = path_cwd(lst_t_var);
	if (!ps1)
	{
		ps1 = ft_strdup("");
		if (!ps1)
			return (NULL);
		prompt = pwd_name(ps1, path, lst_t_var);
		free(ps1);
	}
	else
		prompt = pwd_name(ps1, path, lst_t_var);
	if (!prompt)
	{
		free(path);
		return (NULL);
	}
	return (prompt);
}
