/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 10:15:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/05 16:26:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static void	try_absolut_path(int orig_errfd, char **argv, char **envp)
{
	if (ft_strchr(argv[0], '/'))
	{
		if (is_directory(argv[0]))
			err_endexe(MSHELL_CMD_ISDIR, argv[0], NULL);
		if (is_shell(argv[0]) && access(argv[0], X_OK) && orig_errfd != -1)
			dup2(orig_errfd, STDERR_FILENO);
		if (execve(argv[0], argv, envp))
			err_endexe(MSHELL_FAILURE, "msh: execve", NULL);
	}
}

static void	execmd(int orig_errfd, char **argv, char **envp, char **path)
{
	char	*tmp;
	char	*pathname;
	int		i;

	i = -1;
	try_absolut_path(orig_errfd, argv, envp);
	while (path[++i])
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			err_endexe(MSHELL_FAILURE, "msh: malloc: ", NULL);
		pathname = ft_strjoin(tmp, argv[0]);
		free(tmp);
		if (!pathname)
			err_endexe(MSHELL_FAILURE, "msh: malloc: ", NULL);
		if (is_directory(pathname))
			err_endexe(MSHELL_CMD_ISDIR, argv[0], NULL);
		if (is_shell(pathname) && !access(pathname, X_OK) && orig_errfd != -1)
			dup2(orig_errfd, STDERR_FILENO);
		execve(pathname, argv, envp);
		free(pathname);
		if (errno == EACCES)
			err_endexe(MSHELL_CMD_INVEXE, argv[0], NULL);
	}
	err_endexe(MSHELL_CMD_NOTEXE, argv[0], NULL);
}

static int	exe_setup(int errfd[2], t_cmd *exe, t_list *lst_cmd, t_body *msh)
{
	int	orig_errfd;

	if (errfd[0] != -1)
		close(errfd[0]);
	shell_sigdfl_all();
	orig_errfd = dup(STDERR_FILENO);
	if (errfd[1] != -1 && dup2(errfd[1], STDERR_FILENO) == -1)
	{
		orig_errfd = -1;
		close(errfd[1]);
		shell_redirerr(MSHELL_FAILURE, NULL);
	}
	if (dup2(exe->infd, STDIN_FILENO) == -1)
		err_endexe(MSHELL_FAILURE, "msh: dup2: ", lst_cmd);
	if (dup2(exe->outfd, STDOUT_FILENO) == -1)
		err_endexe(MSHELL_FAILURE, "msh: dup2: ", lst_cmd);
	if (errfd[0] != -1)
		close(errfd[1]);
	exe_endfd(lst_cmd, 0);
	err_endfd(msh->err_fd);
	return (orig_errfd);
}

static void	execmd_ini(int index, char **path, t_list *lst_cmd, t_body *msh)
{
	t_cmd	*exe;
	int		orig_errfd;
	int		errfd[2];

	ft_memset(errfd, -1, 2 * sizeof(int));
	exe = (t_cmd *) lst_cmd->content;
	if (pipe(errfd) == -1)
		shell_redirerr(MSHELL_FAILURE, NULL);
	msh->childs_pid[index] = fork();
	if (!msh->childs_pid[index])
	{
		orig_errfd = exe_setup(errfd, exe, lst_cmd, msh);
		if (exebin_child(exe, msh->envp))
			execmd(orig_errfd, exe->argv, msh->envp, path);
	}
	close(errfd[1]);
	if (msh->childs_pid[index] == -1)
		perror("msh: fork:");
	if (errfd[0] == -1)
		msh->err_fd[index] = -2;
	else
		msh->err_fd[index] = errfd[0];
	exe_endfd(lst_cmd, 1);
}

int	execmd_child(char **path, t_list *lst_cmd, t_body *msh)
{
	int		i;
	char	*last_cmd;
	t_cmd	*exe;

	i = -1;
	while (lst_cmd)
	{
		++i;
		exe = (t_cmd *) lst_cmd->content;
		if (!exe->argv)
		{
			lst_cmd = lst_cmd->next;
			continue ;
		}
		last_cmd = exe->argv[0];
		execmd_ini(i, path, lst_cmd, msh);
		lst_cmd = lst_cmd->next;
	}
	ft_split_free(path);
	if (shell_lastcmd_upd(last_cmd, &(msh->envp), msh->head_envar))
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	return (MSHELL_SUCCESS);
}
