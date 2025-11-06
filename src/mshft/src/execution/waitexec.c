/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitexec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:11:35 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:31:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static const char	*status_sigmsg(int sig)
{
	int						i;
	static const t_sig_msg	sigmsgs[] = {
	{SIGSEGV, "Segmentation fault"},
	{SIGILL, "Illegal instruction"},
	{SIGFPE, "Floating point exception"},
	{SIGABRT, "Aborted"},
	{SIGBUS, "Bus error"},
	{SIGKILL, "Killed"},
	{SIGQUIT, "Quit"},
	{SIGTERM, "Terminated"},
	{SIGTRAP, "Trace/breakpoint trap"},
	{SIGHUP, "Hangup"},
	};

	i = -1;
	while (++i < 10)
		if (sigmsgs[i].sig == sig)
			return (sigmsgs[i].msg);
	return (NULL);
}

static int	check_status(int status, int errfd, int inter, int line)
{
	const char	*msg = NULL;
	int			sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig != SIGINT && sig != SIGPIPE)
		{
			msg = status_sigmsg(sig);
			if (msg)
			{
				if (WCOREDUMP(status))
					ft_fprintf(STDERR_FILENO, "%s (core dumped)\n", msg);
				else
					ft_fprintf(STDERR_FILENO, "%s \n", msg);
			}
		}
		return (128 + sig);
	}
	if (inter || line)
		sig = 0;
	if (WIFEXITED(status))
		return (err_msgfd(WEXITSTATUS(status), errfd, inter, line));
	return (MSHELL_FAILURE);
}

/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * 
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int	waitexec(t_body *msh)
{
	int		i;
	int		line;
	int		inter;
	int		status;
	t_list	*lst_cmd;

	i = -1;
	errno = 0;
	line = msh->line;
	inter = msh->interactive;
	lst_cmd = msh->head_cmd;
	while (lst_cmd)
	{
		if (msh->childs_pid[++i] > 0)
		{
			if (waitpid(msh->childs_pid[i], &status, 0) == -1)
				perror("msh: waitpid");
			msh->exit_no = check_status(status, msh->err_fd[i], inter, line);
		}
		lst_cmd = lst_cmd->next;
	}
	return (MSHELL_SUCCESS);
}
