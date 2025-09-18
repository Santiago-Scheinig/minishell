/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtexp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:30:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * COMMENT PENDING
 */
static char	*pwd_name(char *user, char *path, t_body *minishell)
{
	size_t	len;
	char	*user;
	char	*tmp;

	user = shell_getenv(minishell->envp_lst, "USER");
	if (!user)
		user = "";
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

/**
 * COMMENT PENDING
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

/**
 * COMMENT PENDING
 */
static char	*short_path_name(t_list *envp, char *path)
{
	char	*home;
	int		i;

	home = shell_getenv(envp, "HOME");
	i = ft_strlen(home);
	if (home && home[i - 1] && home[i - 1] != '/' && !access(minishell->home, X_OK))
		path = short_home(home, path);
	if (!path)
		return (NULL);
	return (path);
}

/**
 * COMMENT PENDING
 */
static char	*path_cwd(t_list *envp)
{
	char	*prompt;
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		built_end("pwd", "System failed", NULL, '\0');
		return (NULL);
	}
	prompt = short_path_name(envp, path);
	if (!minishell->home)
	{
		free(path);
		prompt = getcwd(NULL, 0);
	}
	return (prompt);
}

/**
 * COMMENT PENDING
 * NO MINISHELL AS ARGUMENT BUT CHAR ** ENVP!!
 */
char	*shell_pmtexp(t_list *envp)
{
	char	*prompt;
	char	*path;
	char	*ps1;

	ps1 = shell_getenv(envp, "PS1");
	path = path_cwd(envp);
	if (!path)
		return (NULL);
	prompt = pwd_name(ps1, path, minishell);
	if (!prompt)
	{
		free(path);
		return (NULL);
	}
	return (prompt);
}
