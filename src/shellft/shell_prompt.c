/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 14:09:17 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/17 16:28:50 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "bicmd.h"

static char	*pwd_name(char *ps1, char *path, t_body *minishell)
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
	if (!minishell->home)
	{
		free(path);
		path = getcwd(NULL, 0);
	}
	return (path);
}

char *shell_prompt(t_body *minishell)
{
	char	*prompt;
	char	*path;
	char	*ps1;

	ps1 = shell_getenv(minishell->envp_lst, "PS1");
	path = path_cwd(minishell);
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
