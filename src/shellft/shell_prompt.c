/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/16 13:48:19 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "bicmd.h"

static char	*pwd_name(char *user, char *path, t_body *minishell)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(user, ":");
	if (!tmp1)
	{
		free(path);
		free(minishell->prompt);
		return (NULL);
	}
	tmp2 = ft_strjoin(tmp1, path);
	free(tmp1);
	free(path);
	if (!tmp2)
	{
		free(minishell->prompt);
		return (NULL);
	}
	minishell->prompt = ft_strjoin(tmp2, "$ ");
	free(tmp2);
	if (!minishell->prompt)
		return (NULL);
	return (minishell->prompt);
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

static char	*short_path_name(t_body *minishell, char *path)
{
	int	i;

	minishell->home = shell_getenv(minishell->envp_lst, "HOME");
	i = ft_strlen(minishell->home);
	if (minishell->home && minishell->home[i - 1] && minishell->home[i - 1] != '/' && !access(minishell->home, X_OK))
		path = short_home(minishell->home, path);
	if (!path)
		return (NULL);
	return (path);
}

static char *path_cwd(t_body *minishell)
{
	char	*path;

	path = getcwd(NULL, 0);
	if (!path)
	{
		built_end("pwd", "System failed", NULL, '\0');
		return (NULL);
	}
	minishell->home = NULL;
	path = short_path_name(minishell, path);
	return (path);
}

int shell_prompt(t_body *minishell)
{
	char	*user;
	char	*path;

	if (minishell->prompt)
		free(minishell->prompt);
	minishell->prompt = NULL;
	user = getenv("USER");
	path = path_cwd(minishell);
	if (!path)
		return (0);
	if (!pwd_name(user, path, minishell))
		return (0);
	if (!minishell->home)
	{
		if (minishell->prompt)
			free(minishell->prompt);
		minishell->prompt = NULL;
		path = getcwd(NULL, 0);
		if (!pwd_name(user, path, minishell))
			return (0);
	}
	return (1);
}
