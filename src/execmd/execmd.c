/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 17:45:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execmd.h"

/**
 * Pipex failsafe, in case of error, frees all memory that could remain
 * allocated in the main structure.
 * @param env The main environment pipex structure.
 * @param path The enviroment path where execve searches for binaries.
 * @param ft_error The function name where the error in question occured.
 */
int	ft_forcend(t_pipex *env, char **path, char *ft_error)
{
	int	i;

	if (env->cmd)
	{
		i = -1;
		while (env->cmd[++i])
		{
			if (env->cmd[i]->args)
				ft_split_free(env->cmd[i]->args);
			if (env->cmd[i]->pathname)
				free(env->cmd[i]->pathname);
			free(env->cmd[i]);
		}
		free(env->cmd);
	}
	if (path)
		ft_split_free(path);
	if (ft_error)
	{
		perror(ft_error);
		return (-1);
	}
	return (0);
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

/**
 * Initializes the comands needed for here_doc to be executed.
 * Sets the infile to NULL, since we will be reading from the STDIN.
 * Sets the outfile name specified on the main arguments.
 * @param env The main enviroment pipex structure.
 * @param argv The main arguments.
 * @param envp The main enviroment path.
 * @param path The enviroment path where execve searches for binaries.
 * @return Returns 0 on a succesfull execution, and -1 in case of error.
 * @note If any error occurs, the function name and error type will be display
 * on STDERR. 
 */
static int	ft_here_doc(t_pipex *env, char **argv, char **envp, char **path)
{
	int	i;

	i = -1;
	while (++i < env->cmd_count)
	{
		env->cmd[i] = ft_new_cmd(argv[i + 3], (const char **) path);
		if (!env->cmd[i])
			return (ft_forcend(env, path, "Ft_new_cmd"));
	}
	env->infile = NULL;
	env->outfile = argv[env->cmd_count + 3];
	ft_split_free(path);
	return (ft_do_here_doc(env, envp, argv[2]));
}

/**
 * Initializes the comands needed for the pipes to be executed.
 * Sets the infile name specified on the main arguments.
 * Sets the outfile name specified on the main arguments.
 * @param env The main enviroment pipex structure.
 * @param argv The main arguments.
 * @param envp The main enviroment path.
 * @param path The enviroment path where execve searches for binaries.
 * @return Returns 0 on a succesfull execution, and -1 in case of error.
 * @note If any error occurs, the function name and error type will be display
 * on STDERR. 
 */
static int	execute(t_cmd *exe, char **path, t_body *minishell)
{

	if (exe->limitator)
		ft_here_doc(minishell, path);
	else
		ft_do_pipe(env, envp);
}

/**
 * Pipex bonus works in the same way than the original, adding the following
 * extra functionalities:
 * 
 *	- Manage multiple pipes.
 *	- Accepts << and >> operators, when here\_doc is specified on the main
 *	argument as argv[1]; 
 */
int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	char	**path;
	int		is_single;

	cmd_lst = minishell->cmd_lst;
	is_single = 0;
	if (!cmd_lst->next)
		is_single = 1;
	while (cmd_lst)
	{
		exe = (t_cmd *) cmd_lst->content;
		path_bin = ft_check_path(minishell->envp);
		if (!path_bin)
			sigend(minishell, 1);
		exe->pathname = cmd_path(exe->argv[0], path);
		if (!exe->pathname)
		{
			strerror(0);
			cmd_lst = cmd_lst->next;
			continue;
		}
		execute(exe, path, minishell);
		cmd_lst = cmd_lst->next;
	}
	ft_split_free(path);
}
