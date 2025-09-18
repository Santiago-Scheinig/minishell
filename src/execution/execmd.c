/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:33:12 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

// static void	exe_setup()

static void	exe_child(t_cmd *exe, t_cmd *exe_next, char **path, t_body *minishell)
{
	static int	i;
	int			pipefd[2];

	if (fd_setexe(exe, exe_next, pipefd) || pipe(pipefd))
		forcend(minishell, "pipe", MSHELL_FAILURE)//sets exein and exeout, if exein is heredoc, executes a reader, and saves heredoc[1] on exein. If exeout is default, it becomes replaced by pipefd[1]
	minishell->waitpid_list[i] = fork();
	if (!minishell->waitpid_list[i] && !(exe->fd.exein < 0))
	{
		// setsigmask(); sets the list of signals saved on minishell to default.
		if (dup2(exe->fd.exein, STDIN_FILENO) == -1
			|| dup2(exe->fd.exeout, STDOUT_FILENO) == -1)
			perror("Dup2");//childend();
		if (exe_verification(exe, path))//verifies cmd before dup or execution
		{
			fd_endexe(exe, pipefd);
			perror("msg");//childend();
		}
		fd_endexe(exe, pipefd);//closes both exein and exeout.
		if (execve(exe->pathname, exe->argv, path))
			perror("Execve");//childend();
	}
	i++;
	fd_endexe(exe, pipefd);//closes all the fds inside of the cmd and fd file structure that being infd outfd (only if they arent standard), heredoc[1] if valid, then fd.exein fd.exeout.
}

// static void	exe_built()

int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;

	cmd_lst = minishell->cmd_lst;
	if (!cmd_lst->next)
		//exe_built();//execute builtin only if it modifies the parent data
	if (g_signal_received)
	{
		g_signal_received = 0;
		return (MSHELL_SIG_HANDLR);
	}
	path = exe_setup(minishell);//returns path char ** and sets waitpid array inside minishell;
	while (cmd_lst)
	{
		exe = (t_cmd *) cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = (t_cmd *) cmd_lst->next->content;
		exe_child(exe, exe_next, path);
		cmd_lst = cmd_lst->next;
	}
	if (path)
		ft_split_free(path);
}
