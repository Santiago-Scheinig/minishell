/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/19 21:30:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static char	**exe_setup(t_body *minishell)
{
	char	**path;
	int		cmd_len;
	
	cmd_len = ft_lstsize(minishell->cmd_lst);
	minishell->childs_pid = calloc(cmd_len, sizeof(pid_t));
	if (!minishell->childs_pid)
		forcend(minishell, "malloc", MSHELL_FAILURE);
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

static int	exe_check(t_cmd *exe, char **path)
{
	if (!exe->argv || !exe->argv[0])
		return (MSHELL_FAILURE);
	exe->pathname = cmd_getpath(exe->argv[0], path);//should print all errors
	if (!exe->pathname)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

static void	exe_child(t_cmd *exe, t_cmd *exe_next, char **path, t_body *minishell)
{
	static int	i;
	int			pipefd[2];

	if (pipe(pipefd))
		forcend(minishell, "pipe", MSHELL_FAILURE);
	fd_setexe(exe, exe_next, pipefd);
	minishell->childs_pid[i] = fork();
	if (!minishell->childs_pid[i] && !(exe->fd.exein < 0))
	{
		if (sigdfl())
			exit(MSHELL_FAILURE);//childend();
		if (exe_check(exe, path))
		{
			fd_endexe(exe, pipefd);
			perror("msg");//childend();
			exit(MSHELL_FAILURE);
		}
		if (dup2(exe->fd.exein, STDIN_FILENO) == -1
			|| dup2(exe->fd.exeout, STDOUT_FILENO) == -1)
		{
			perror("Dup2");//childend();
			exit(MSHELL_FAILURE);
		}
		fd_endexe(exe, pipefd);
		if (execve(exe->pathname, exe->argv, path))
		{
			perror("Execve");//childend();
			exit(MSHELL_FAILURE);
		}
	}
	i++;
	fd_endexe(exe, pipefd);
}

// static void	exe_built()

int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;

	cmd_lst = minishell->cmd_lst;
	// if (!cmd_lst->next)
		//exe_built();//execute builtin only if it modifies the parent data
	if (g_signal_received)
	{
		g_signal_received = 0;
		return (MSHELL_SIG_HANDLR);
	}
	path = exe_setup(minishell);
	while (cmd_lst)
	{
		exe = (t_cmd *) cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = (t_cmd *) cmd_lst->next->content;
		exe_child(exe, exe_next, path, minishell);
		cmd_lst = cmd_lst->next;
	}
	ft_split_free(path);
	return (MSHELL_SUCCESS);
}
