/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/21 17:23:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * Creates and allocates a STRING with the definitive path to a cmd binary.
 * 
 * @param cmd The name of the command binary to find.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new STRING or NULL if the allocation failed or
 * the cmd can't be access or found as binary on path.
 */
static int exe_getpath(char *cmd, char **path, char **pathname)
{
	char	*tmp;
	int		i;

	i = -1;
	if (ft_strchr(cmd, '/'))
	{
		(*pathname) = cmd;
		if (!access(cmd, X_OK))
			return (MSHELL_SUCCESS);
		return (MSHELL_CMD_INVEXE);//print no access error!
	}
	while (path[++i] && cmd)
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (MSHELL_FAILURE);//this are malloc error... i need to end the child setting errno! So the parent know what failed
		(*pathname) = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!(*pathname))
			return (MSHELL_FAILURE);//this are malloc error... i need to end the child setting errno!
		if (!access((*pathname), X_OK))
			return (MSHELL_SUCCESS);
		free((*pathname));
	}
	return (MSHELL_CMD_NOTEXE);//print command not found
}

static char	**exe_setup(t_body *minishell)
{
	char	**path;
	int		cmd_len;
	
	cmd_len = ft_lstsize(minishell->cmd_lst);
	minishell->childs_pid = calloc(cmd_len, sizeof(pid_t));
	if (!minishell->childs_pid)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	if (setup_pipeline(minishell->cmd_lst))
	{
		if (errno == ENOMEM)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		return (NULL);
	}
	path = setup_path((const char **) minishell->envp);
	if (!path)
	{
		path = calloc(1, sizeof(char *));
		if (!path)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		return (path);
	}
	return (path);
}

static void	exe_child(t_cmd *exe, char **path, pid_t *child, char **envp)
{
	int error;

	(*child) = fork();
	if (!(*child) && !(exe->infd < 0))
	{
		sigdfl();
		if (!exe->argv || !exe->argv[0])
			exit (MSHELL_FAILURE);
		error = exe_getpath(exe->argv[0], path, &(exe->pathname));
		if (error)
			exit (error);
		if (dup2(exe->infd, STDIN_FILENO) == -1
			|| dup2(exe->outfd, STDOUT_FILENO) == -1)
		{
			ft_printfd(2, "msh: %s: Bad file descriptor", exe->argv[0]);
			exit(MSHELL_FAILURE);
		}
		fd_endexe(exe);
		//if (exe_built())
		// {
		if (execve(exe->pathname, exe->argv, envp))
			exit(MSHELL_FAILURE);//childend();
		// }
	}
	fd_endexe(exe);
}

int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;
	int		i;

	cmd_lst = minishell->cmd_lst;
	i = -1;
	if (!cmd_lst->next)
		i = exe_built((t_cmd *) minishell->cmd_lst->content, minishell);
	if (i == -1)
	{
		path = exe_setup(minishell);
		if (!path)
			return (MSHELL_FAILURE);
		while (cmd_lst)
		{
			exe = (t_cmd *) cmd_lst->content;
			exe_next = NULL;
			if (cmd_lst->next)
			exe_next = (t_cmd *) cmd_lst->next->content;
			exe_child(exe, path, &(minishell->childs_pid[++i]), minishell->envp);
			cmd_lst = cmd_lst->next;
		}
		ft_split_free(path);
	}
	return (MSHELL_SUCCESS);
}
