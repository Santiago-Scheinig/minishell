/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:22:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static void	child_cmd(char **argv, char **envp, char **path)
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

static char	**exe_setup(t_body *minishell)
{
	char	**path;
	int		cmd_len;

	cmd_len = ft_lstsize(minishell->lst_t_cmd);
	minishell->childs_pid = calloc(cmd_len, sizeof(pid_t));
	if (!minishell->childs_pid)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	minishell->err_fd = calloc(cmd_len, sizeof(int));
	if (!minishell->err_fd)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	if (setup_pipeline(minishell->lst_t_cmd))
	{
		if (errno == ENOMEM)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		minishell->exit_no = MSHELL_FAILURE;
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
	if (minishell->interactive)
		shell_sigign();
	return (path);
}

int	exend(int exit_no, char *err_msg, t_list *lst_t_cmd)
{
	if (err_msg)
	{
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
		err_msg = strerror(errno);
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	}
	if (lst_t_cmd)
		fd_endexe(lst_t_cmd, 0);
	if (errno == ENOENT)
		exit(MSHELL_CMD_NOTEXE);
	else if (errno == EACCES || errno == EPERM || errno == ENOEXEC)
		exit(MSHELL_CMD_INVEXE);
	else if (errno)
		exit(exit_no);
	exit(exit_no);
}

static void	exe_init(int errfd[2], t_cmd *exe, t_list *lst_t_cmd)
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

static int	exe_child(t_list *lst_t_cmd, char **path, pid_t *child, char **envp)
{
	t_cmd	*exe;
	int		errfd[2];

	if (pipe(errfd) == -1)
		redirend(NULL, MSHELL_FAILURE);
	(*child) = fork();
	if (!(*child))
	{
		exe = (t_cmd *) lst_t_cmd->content;
		exe_init(errfd, exe, lst_t_cmd);
		if (child_bicmd(exe, envp))
			child_cmd(exe->argv, envp, path);
	}
	close(errfd[1]);
	fd_endexe(lst_t_cmd, 1);
	return (errfd[0]);
}

int	execmd(t_body *msh)
{
	t_list	*lst_t_cmd;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;
	int		i;

	i = -1;
	lst_t_cmd = msh->lst_t_cmd;
	if (!lst_t_cmd->next)
		i = father_bicmd((t_cmd *) lst_t_cmd->content, msh);
	if (i)
	{
		i = -1;
		path = exe_setup(msh);
		if (!path)
			return (MSHELL_FAILURE);
		while (lst_t_cmd)
		{
			++i;
			exe = (t_cmd *) lst_t_cmd->content;
			exe_next = NULL;
			if (lst_t_cmd->next)
				exe_next = (t_cmd *) lst_t_cmd->next->content;
			//save last cmd
			msh->err_fd[i] = exe_child(lst_t_cmd, path, &(msh->childs_pid[i]), msh->envp);
			lst_t_cmd = lst_t_cmd->next;
		}
		ft_split_free(path);
	}
	else
		msh->exit_no = i;
	return (MSHELL_SUCCESS);
}
