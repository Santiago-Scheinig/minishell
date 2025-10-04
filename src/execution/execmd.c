/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 16:17:25 by ischeini         ###   ########.fr       */
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

	cmd_len = ft_lstsize(minishell->cmd_lst);
	minishell->childs_pid = calloc(cmd_len, sizeof(pid_t));
	if (!minishell->childs_pid)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	minishell->err_fd = calloc(cmd_len, sizeof(int));
	if (!minishell->err_fd)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	if (setup_pipeline(minishell->cmd_lst))
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
		sigign();
	return (path);
}

int	exend(int exit_no, char *err_msg, t_list *cmd_lst)
{
	if (err_msg)
	{
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
		err_msg = strerror(errno);
		write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	}
	if (cmd_lst)
		fd_endexe(cmd_lst, 0);
	if (errno == ENOENT)
		exit(MSHELL_CMD_NOTEXE);
	else if (errno == EACCES || errno == EPERM || errno == ENOEXEC)
		exit(MSHELL_CMD_INVEXE);
	else if (errno)
		exit(exit_no);
	exit(exit_no);
}

static void	exe_init(int errfd[2], t_cmd *exe, t_list *cmd_lst)
{
	close(errfd[0]);
	sigdfl();
	//dup2(errfd[1], STDERR_FILENO);
	if (!exe->argv || !exe->argv[0])
		exend(MSHELL_SUCCESS, NULL, cmd_lst);
	if (dup2(exe->infd, STDIN_FILENO) == -1
		|| dup2(exe->outfd, STDOUT_FILENO) == -1)
		exend(MSHELL_FAILURE, "msh: dup2: ", cmd_lst);
	close(errfd[1]);
	fd_endexe(cmd_lst, 0);
}

static int	exe_child(t_list *cmd_lst, char **path, pid_t *child, char **envp)
{
	t_cmd	*exe;
	int		errfd[2];

	if (pipe(errfd) == -1)
		redirend(NULL, MSHELL_FAILURE);
	(*child) = fork();
	if (!(*child))
	{
		exe = (t_cmd *) cmd_lst->content;
		exe_init(errfd, exe, cmd_lst);
		if (child_bicmd(exe, envp))
			child_cmd(exe->argv, envp, path);
	}
	close(errfd[1]);
	fd_endexe(cmd_lst, 1);
	return (errfd[0]);
}

int	execmd(t_body *msh)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;
	int		i;

	i = -1;
	cmd_lst = msh->cmd_lst;
	if (!cmd_lst->next)
		i = father_bicmd((t_cmd *) cmd_lst->content, msh);
	if (i)
	{
		i = -1;
		path = exe_setup(msh);
		if (!path)
			return (MSHELL_FAILURE);
		while (cmd_lst)
		{
			++i;
			exe = (t_cmd *) cmd_lst->content;
			exe_next = NULL;
			if (cmd_lst->next)
				exe_next = (t_cmd *) cmd_lst->next->content;
			//save last cmd
			msh->err_fd[i] = exe_child(cmd_lst, path, &(msh->childs_pid[i]), msh->envp);
			cmd_lst = cmd_lst->next;
		}
		ft_split_free(path);
	}
	else
		msh->exit_no = i;
	return (MSHELL_SUCCESS);
}
