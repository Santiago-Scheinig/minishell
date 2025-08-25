/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_path_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/23 19:36:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"



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
 * Creates and allocates a CMD strucure.
 * @param cmd The name of the command.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new CMD structure or NULL if the allocation
 * failed or the cmd can't be found as binary on path.
 */
t_cmd	*ft_new_cmd()
{
	new->pathname = ft_get_cmd_path(new->args[0], path);
	if (!new->pathname)
	{
		ft_split_free(new->args);
		free(new);
		return (NULL);
	}
	return (new);
}

/**
 * Returns the amount of comands on the main arguments, following the format
 * specified for pipex.
 * @param argv The main arguments.
 */
int	ft_get_cmd_count(char **argv)
{
	int	i;

	i = 0;
	if (!argv || !argv[0])
		return (i);
	while (argv[i])
		i++;
	if (!ft_strncmp(argv[1], "here_doc\0", 10))
		return (i - 4);
	return (i - 3);
}
