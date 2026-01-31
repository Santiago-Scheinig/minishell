/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitexec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:11:35 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:36:39 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * @brief	Returns a string describing a signal code.
 *
 *			Matches sig against a static array of known signals
 *			and returns the corresponding message string.
 *
 * @param	sig	Signal number.
 *
 * @return	Message string if found, or NULL otherwise.
 */
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

/**
 * @brief	Checks a child process exit status and prints errors.
 *
 *			Handles both normal exit codes and signals. Prints
 *			signal messages (with "core dumped" if applicable)
 *			or forwards exit codes to err_msgfd().
 *
 * @param	status	Status from waitpid().
 * @param	errfd	File descriptor for error messages.
 * @param	inter	Interactive mode flag.
 * @param	line	Input line number for reporting.
 *
 * @return	Exit code for the child process or MSHELL_FAILURE.
 */
int	check_status(int status, int errfd, int inter, int line)
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
	if (WIFEXITED(status))
		return (err_msgfd(WEXITSTATUS(status), errfd, inter, line));
	return (MSHELL_FAILURE);
}

/**
 * @brief	Waits for all child processes and updates shell status.
 *
 *			Iterates through msh->childs_pid, waits for each child,
 *			and updates msh->exit_no and msh->input_result. Handles
 *			errors from waitpid().
 *
 * @param	msh		Shell context containing child pids, error fds,
 *					line number, and interactive mode flag.
 *
 * @return	MSHELL_SUCCESS after all children have been waited for.
 *
 * @note	Non-waited children may become zombies if waitpid fails.
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
			if (msh->exit_no)
				msh->input_result = MSHELL_FAILURE;
			msh->childs_pid[i] = -1;
		}
		lst_cmd = lst_cmd->next;
	}
	return (MSHELL_SUCCESS);
}
