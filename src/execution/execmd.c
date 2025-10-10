/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 10:15:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 10:42:40 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static void	execmd(char **argv, char **envp, char **path)
{
	char	*tmp;
	char	*pathname;
	int		i;

	i = -1;
	if (ft_strchr(argv[0], '/'))
		if (execve(argv[0], argv, envp))
			exend(MSHELL_FAILURE, "msh: execve: ", NULL);
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			exend(MSHELL_FAILURE, "msh: malloc: ", NULL);
		pathname = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (!pathname)
			exend(MSHELL_FAILURE, "msh: malloc: ", NULL);
		// if(im_shell() && access(argv[0], X_OK))
		//	dup2(orig, STDERR_FILENO);
		if (execve(pathname, argv, envp))
		{
			free(pathname);
			if (errno == EACCES)
				exend(MSHELL_CMD_INVEXE, NULL, NULL);
		}
	}
	exend(MSHELL_CMD_NOTEXE, NULL, NULL);
}

static void	execmd_setup(int errfd[2], t_cmd *exe, t_list *lst_t_cmd)
{
	close(errfd[0]);
	shell_sigdfl();
	//dup(STDERR_FILENO);
	//dup2(errfd[1], STDERR_FILENO);
	if (!exe->argv || !exe->argv[0])
		exend(MSHELL_SUCCESS, NULL, lst_t_cmd);
	if (dup2(exe->infd, STDIN_FILENO) == -1
		|| dup2(exe->outfd, STDOUT_FILENO) == -1)
		exend(MSHELL_FAILURE, "msh: dup2: ", lst_t_cmd);
	close(errfd[1]);
	fd_endexe(lst_t_cmd, 0);//i need to close all other errfd that arent from this child...
}

static int	execmd_ini(t_list *lst_cmd, char **path, pid_t *child, char **envp)
{
	t_cmd	*exe;
	int		errfd[2];

	exe = (t_cmd *) lst_cmd->content;
	if (pipe(errfd) == -1)
		redirend(NULL, MSHELL_FAILURE);
	(*child) = fork();
	if (!(*child))
	{
		execmd_setup(errfd, exe, lst_cmd);
		if (exebin_child(exe, envp))
			execmd_child(exe->argv, envp, path);
	}
	if ((*child) == -1)
		//whats the error here?
	close(errfd[1]);
	fd_endexe(lst_cmd, 1);
	return (errfd[0]);
}

int	execmd_child(t_list *lst_cmd, char **path, t_body *msh)
{
	int		i;
	char	*last_cmd;
	char	**path;
	t_cmd	*exe;

	i = -1;
	while (lst_cmd)
	{
		++i;
		exe = (t_cmd *) lst_cmd->content;
		if (!exe->argv)
		{
			lst_cmd = lst_cmd->next;
			continue;
		}
		last_cmd = exe->argv[0];
		msh->err_fd[i] = execmd_ini(lst_cmd, path, &(msh->childs_pid[i]), msh->envp);//pidarr
		lst_cmd = lst_cmd->next;
	}
	ft_split_free(path);
	return (MSHELL_SUCCESS);//function to add last cmd saved on char * lastcmd (so no allocations are made during execution);
}

