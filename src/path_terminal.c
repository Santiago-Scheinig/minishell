/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_terminal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/09 15:33:27 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	free(path);
	if (!tmp2)
	{
		free(minishell->prompt);
		return (NULL);
	}
	minishell->prompt = ft_strjoin(tmp2, "$ ");
	if (!minishell->prompt)
		return (NULL);
	return (minishell->prompt);
}

static char	*

static char	*short_path_name(char *path, char *user)
{
	size_t	rest;
	char	*short_path;
	char	*tmp;

	short_path = NULL;
	tmp = malloc((6 + ft_strlen(user) + 1) * sizeof(char));
	if (!tmp)
	{
		perror("malloc");
		free(path);
		return (NULL);
	}
	ft_strlcpy(tmp, "/home/", 7);
	ft_strlcat(tmp, user, 6 + ft_strlen(user) + 1);
	if (ft_strncmp(path, tmp, ft_strlen(tmp)) == 0)
	{
		rest = ft_strlen(path + ft_strlen(tmp));
		short_path = malloc(2 + rest + 1);
		if (!short_path)
		{
			perror("malloc");
			free(tmp);
			free(path);
			return (NULL);
		}
		short_path[0] = '~';
		ft_strlcpy(short_path + 1, path + ft_strlen(tmp), rest + 1);
		free(tmp);
		free(path);
		return (short_path);
	}
	free(tmp);
	return (path);
}

static char	*path_cwd(char *user)
{
	char	*path;

	path = malloc(PATH_MAX * sizeof(char));
	if (!path)
	{
		perror("malloc");
		return (NULL);
	}
	if (!getcwd(path, PATH_MAX))
	{
		perror("pwd");
		free(path);
		return (NULL);
	}
	if (ft_strlen(path) > 5)
		path = short_path_name(path, user);
	return (path);
}

int	path_minishell(t_body *minishell)
{
	size_t	size;
	char	*user;
	char	*path;

	user = getenv("USER");
	if (!user)
	{
		perror("USER no define");
		return (0);
	}
	path = path_cwd(user);
	if (!path)
		return (0);
	size = ft_strlen(path) + 4 + ft_strlen(user);
	minishell->prompt = malloc(size * sizeof(char));
	if (!minishell->prompt)
	{
		free(path);
		return (0);
	}
	if (!pwd_name(user, path, minishell))
		return (0);
	return (1);
}
