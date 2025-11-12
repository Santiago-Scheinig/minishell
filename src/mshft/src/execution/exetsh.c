/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exetsh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 16:19:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:34:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * @brief	Closes all file descriptors in the error array.
 *
 *			Iterates through errfd until -1 is encountered and closes
 *			each fd greater than 2 (STDIN/STDOUT/STDERR are preserved).
 *
 * @param	errfd	Array of file descriptors to close.
 */
void	err_endfd(int *errfd)
{
	int	i;

	i = -1;
	while (errfd[++i] != -1)
		if (errfd[i] > 2)
			close(errfd[i]);
}

/**
 * @brief	Prints error messages read from a file descriptor.
 *
 *			Reads lines from errfd and writes them to STDERR. Handles
 *			different formatting depending on interactive mode and
 *			line numbers.
 *
 * @param	status	Exit status to return.
 * @param	errfd	File descriptor to read error messages from.
 * @param	interactive	Flag indicating interactive shell mode.
 * @param	line	Input line number for formatting.
 *
 * @return	Original status parameter.
 */
int	err_msgfd(int status, int errfd, int interactive, int line)
{
	char	*err_line;
	int		i;

	err_line = NULL;
	if (errfd > 2)
		err_line = ft_get_next_line(errfd);
	else
		return (status);
	while (err_line)
	{
		if (!interactive)
		{
			i = -1;
			write(STDERR_FILENO, "msh: ", 5);
			ft_fprintf(STDERR_FILENO, "line %i: %s", line, &err_line[++i]);
		}
		else
			ft_fprintf(STDERR_FILENO, "%s", err_line);
		free(err_line);
		err_line = ft_get_next_line(errfd);
	}
	close(errfd);
	return (status);
}

/**
 * @brief	Handles execution errors and terminates the process.
 *
 *			Prints descriptive error messages based on exit_no or
 *			perror() if err_msg is provided. Closes any open fds
 *			from lst_cmd before exiting.
 *
 * @param	exit_no	Exit code indicating type of error.
 * @param	err_msg	Error message prefix, or NULL to use perror().
 * @param	lst_cmd	Command node whose file descriptors should be closed.
 */
int	err_endexe(int exit_no, char *err_msg, t_list *lst_cmd)
{
	const char	*msg;

	if (exit_no == MSHELL_CMD_INVEXE)
		msg = "permission denied";
	if (exit_no == MSHELL_CMD_NOTEXE)
		msg = "command not found";
	if (exit_no == MSHELL_CMD_ISDIR)
		msg = "is a directory";
	if (exit_no == MSHELL_CMD_INVEXE || exit_no == MSHELL_CMD_NOTEXE
		|| exit_no == MSHELL_CMD_ISDIR)
		ft_fprintf(STDERR_FILENO, "%s: %s\n", err_msg, msg);
	else if (err_msg)
		perror(err_msg);
	if (lst_cmd)
		exe_endfd(lst_cmd, 0);
	if (errno == ENOENT)
		exit(MSHELL_CMD_NOTEXE);
	else if (errno == EACCES || errno == EPERM || errno == ENOEXEC)
		exit(MSHELL_CMD_INVEXE);
	else if (errno)
		exit(exit_no);
	exit(exit_no);
}
