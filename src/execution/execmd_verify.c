/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd_verify.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:33:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * Verifies the existence of path enviroment.
 * @param envp The main enviroment where to search for the path enviroment.
 * @return If path exists, returns a CHAR ** with each definitive path specified
 * on it (divided by ':'); If not, returns NULL.
 */
char	**ft_check_path(const char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp("PATH=", envp[i], 5))
			return (ft_split(&envp[i][5], ':'));
	return (NULL);
}

/**
 * Creates and allocates a STRING with the definitive path to a cmd binary.
 * @param cmd The name of the command binary to find.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new STRING or NULL if the allocation failed or
 * the cmd can't be access or found as binary on path.
 */
static char	*ft_get_cmd_path(const char *cmd, const char **path)
{
	char	*cmd_pathname;
	char	*tmp;
	int		i;

	i = -1;
	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	while (path[++i] && cmd)
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		cmd_pathname = ft_strjoin(tmp, cmd);
		if (!cmd_pathname)
			return (NULL);
		free(tmp);
		if (!access(cmd_pathname, X_OK))
			return (cmd_pathname);
		free(cmd_pathname);
	}
	return (ft_strdup(""));
}

static char	*exe_verification(t_cmd *exe, char **path)
{
	char	*pathname;

	if (exe->infile < 0)
	{
		if (exe->infile != -2 || !exe->limitator)
			return (NULL);
		else if (exe->heredoc[0] < 0 || exe->heredoc[1] < 0)
			return (NULL);
	}
	else if (exe->outfile < 0)
		return (NULL);
	pathname = cmd_pathname(exe->argv[0], path_env);
	if (!pathname)
	{
		strerror(0);
		return (NULL);
	}
	return (pathname);
}