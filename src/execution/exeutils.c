/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 10:34:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

int	exerr(int exit_no, char *err_msg, t_list *lst_t_cmd)
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

int	getbin(char *cmd)
{
	if (!cmd)
		return (BINCMD_NOEXE);
	if (!ft_strncmp(cmd, "export", 7))
		return (BINCMD_EXPORT);
	else if (!ft_strncmp(cmd, "cd", 3))
		return (BINCMD_CD);
	else if (!ft_strncmp(cmd, "env", 4))
		return (BINCMD_ENV);
	else if (!ft_strncmp(cmd, "pwd", 4))
		return (BINCMD_PWD);
	else if (!ft_strncmp(cmd, "echo", 5))
		return (BINCMD_ECHO);
	else if (!ft_strncmp(cmd, "unset", 6))
		return (BINCMD_UNSET);
	else if (!ft_strncmp(cmd, "exit", 5))
		return (BINCMD_EXIT);
	else if (ft_strchr(cmd, '='))
		return (BINCMD_IMPORT);
	return (BINCMD_NOEXE);
}

void	fd_endexe(t_list *cmd_lst, pid_t child)
{
	t_cmd	*exe;

	exe = (t_cmd *) cmd_lst->content;
	if (exe->infd > 2)
		close(exe->infd);
	if (exe->outfd > 2)
		close(exe->outfd);
	if (!child)
	{
		while (cmd_lst)
		{
			exe = (t_cmd *) cmd_lst->content;
			if (exe->pipefd > 2)
				close(exe->pipefd);
			cmd_lst = cmd_lst->next;
		}
	}
}

/**
 * Connects all the file descriptors between the cmd to execute
 * and the following cmd.
 * 
 * @param exe A pointer to the cmd to be executed in this current iteration.
 * @param exe_next A pointer to the next cmd to be executed.
 * @param pipefd An array of INT which saves an already initialized pipe().
 * @note Also handles heredoc redirections, saving the reading end of that
 * pipe as the [exe] infile, and closing the previous one.
 */
int	fd_setexe(t_cmd *exe, t_cmd *exe_next)
{
	int	pipefd[2];

	if (exe_next)
	{
		if (pipe(pipefd))
			return (MSHELL_FAILURE);
		if (exe->outfd == STDOUT_FILENO)
			exe->outfd = pipefd[1];
		else
			close(pipefd[1]);
		if (exe_next->infd == STDIN_FILENO)
		{
			exe_next->infd = pipefd[0];
			exe->pipefd = pipefd[0];
		}
		else
			close(pipefd[0]);
	}
	return (MSHELL_SUCCESS);
}
