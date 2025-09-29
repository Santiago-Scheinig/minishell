/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/29 12:21:34 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static char	*pwd_name(char *ps1, char *path, t_list *envp_lst)
{
	size_t	len;
	char	*user;
	char	*tmp;

	user = shell_getenv(envp_lst, "USER");
	if (!user)
		user = NULL;
	len = prompt_len(ps1, user, path);
	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
	{
		perror("malloc");
		return (NULL);
	}
	tmp = transform_format(tmp, ps1, user, path);
	free(path);
	return (tmp);
}

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
			perror("malloc");
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

static char	*short_path_name(t_list *envp, char *path)
{
	char	*home;
	int		i;

	home = shell_getenv(envp, "HOME");
	if (!home)
		return (path);
	i = ft_strlen(home);
	if (home && home[i - 1] && home[i - 1] != '/' && !access(home, X_OK))
		path = short_home(home, path);
	if (!path)
		return (NULL);
	return (path);
}

static char	*path_cwd(t_list *envp)
{
	char	*prompt;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
		path = ft_strdup(shell_getenv(envp, "PWD"));
	if (!path)
		return (NULL);
	prompt = short_path_name(envp, path);
	if (!prompt)
		return (NULL);
	return (prompt);
}

/**
 * Builds and returns the expanded shell prompt string.
 * 
 * @param envp Linked list of environment variables (t_var nodes).
 * 
 * Reads the PS1 format from the environment, determines the current working
 * directory (falls back to PWD when getcwd fails), shortens the home path to
 * '~'
 * when applicable, and expands PS1 escapes into a newly allocated prompt
 * string.
 * 
 * @return Newly allocated prompt string on success, or NULL on failure.
 * @note Caller is responsible for freeing the returned string.
 * @note If PS1 is not defined an empty format is used. On allocation failure
 *       the function returns NULL and partial allocations are freed internally
 */
char	*shell_pmtexp(t_list *envp)
{
	char	*prompt;
	char	*path;
	char	*ps1;

	ps1 = shell_getenv(envp, "PS1");
	path = path_cwd(envp);
	if (!ps1)
	{
		ps1 = ft_strdup("");
		if (!ps1)
			return (NULL);
		prompt = pwd_name(ps1, path, envp);
		free(ps1);
	}
	else
		prompt = pwd_name(ps1, path, envp);
	if (!prompt)
	{
		free(path);
		return (NULL);
	}
	return (prompt);
}
