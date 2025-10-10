/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitexec.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:11:35 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 10:43:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
 
static const char	*status_sigmsg(int sig)
{
	int						i;
	static const t_sig_msg	sigmsgs[] = {
    { SIGSEGV, "Segmentation fault" },
    { SIGILL,  "Illegal instruction" },
    { SIGFPE,  "Floating point exception" },
    { SIGABRT, "Aborted" },
    { SIGBUS,  "Bus error" },
    { SIGKILL, "Killed" },
    { SIGQUIT, "Quit" },
    { SIGTERM, "Terminated" },
    { SIGTRAP, "Trace/breakpoint trap" },
    { SIGHUP,  "Hangup" },
	};

	i = -1;
	while (++i < 10)
		if (sigmsgs[i].sig == sig)
			return (sigmsgs[i].msg);
	return (NULL);
}

static void	print_status(t_cmd *exe, int exit_no)
{
	const char	*msg;

	if (exit_no == MSHELL_CMD_INVEXE)
		msg = "permission denied";
	if (exit_no == MSHELL_CMD_NOTEXE)
		msg = "command not found";
	ft_printfd(STDERR_FILENO, "msh: %s: %s\n", exe->argv[0], msg);
}

static int	status_exitmsg(int status, int errfd)
{
	char	*err_line;

	if (status)
	{
		err_line = get_next_line(errfd);
		if (!err_line)
		{
/* 			if (errno == ENOMEM)
				msh->exit_ft = "malloc";//forcend
			else if (errno)
				msh->exit_ft = "read";//forcend */
			return (status);
		}
		ft_printfd(STDERR_FILENO, "%s\n", err_line);
		close(errfd);
	}
	return (status);
}

static int	check_status(int status, int errfd)
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
					ft_printfd(STDERR_FILENO, "%s (core dumped)\n", msg);
				else
					ft_printfd(STDERR_FILENO, "%s \n", msg);
			}
		}
		return (128 + sig);
	}
	if (WIFEXITED(status))
		return (status_exitmsg(WEXITSTATUS(status), errfd));
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
int	waitexec(t_body *minishell)
{
	int		i;
	int 	status;
	t_list	*lst_t_cmd;

	i = -1;
	errno = 0;
	lst_t_cmd = minishell->lst_t_cmd;
	while (lst_t_cmd)
	{
		if (minishell->childs_pid[++i] > 0)
		{
			if (waitpid(minishell->childs_pid[i], &status, 0) == -1)
				perror("msh: waitpid");
			minishell->exit_no = check_status(status, minishell->err_fd[i]);//no check status, only errpipe writing
			if (minishell->exit_no == MSHELL_CMD_INVEXE
				|| minishell->exit_no == MSHELL_CMD_NOTEXE)
				print_status((t_cmd *) lst_t_cmd->content, minishell->exit_no);
		}
		lst_t_cmd = lst_t_cmd->next;
	}
	if (minishell->interactive)
		shell_sigint();
	return (MSHELL_SUCCESS);
}
