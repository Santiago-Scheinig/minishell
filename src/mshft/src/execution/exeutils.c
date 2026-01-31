/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:05:22 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * @brief	Checks if a given path is a directory.
 *
 *			Uses stat() to determine if the path exists and is a
 *			directory.
 *
 * @param	path	Path to check.
 *
 * @return	true if path is a directory, false otherwise.
 */
int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (false);
	if (S_ISDIR(path_stat.st_mode))
		return (true);
	return (false);
}

/**
 * @brief	Determines if a command corresponds to a known shell.
 *
 *			Checks the basename of cmd against a static list of known
 *			shell names.
 *
 * @param	cmd	Command string to check.
 *
 * @return	true if cmd is a recognized shell, false otherwise.
 */
int	is_shell(const char *cmd)
{
	int					i;
	int					cmd_len;
	int					shell_len;
	const char			*aux;
	static const char	*shells[] = {
		"sh", "bash", "dash", "zsh", "ksh", "mksh",
		"ash", "csh", "tcsh", "fish", "msh", "msh_bonus", NULL
	};

	i = -1;
	aux = ft_strrchr(cmd, '/');
	if (aux)
		cmd = &aux[1];
	cmd_len = ft_strlen(aux + 1);
	while (shells[++i])
	{
		shell_len = ft_strlen(shells[i]);
		if (cmd_len == shell_len && !ft_strncmp(cmd, shells[i], cmd_len))
			return (true);
	}
	return (false);
}

/**
 * @brief	Identifies which built-in command a string represents.
 *
 *			Matches cmd against known built-in commands like export,
 *			cd, env, pwd, echo, unset, exit, or import (assignment).
 *
 * @param	cmd	Command string.
 *
 * @return	Identifier for the built-in, or BINCMD_NOEXE if none.
 */
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

/**
 * @brief	Closes file descriptors associated with a command or pipeline.
 *
 *			Closes exe->infd and exe->outfd if >2. If child == 0, also
 *			closes pipefds for all commands in the list.
 *
 * @param	lst_cmd	Command or list of commands.
 * @param	child	Flag indicating child process (0 closes all pipefds).
 */
void	exe_endfd(t_list *lst_cmd, pid_t child)
{
	t_cmd	*exe;

	exe = (t_cmd *) lst_cmd->content;
	if (exe->infd > 2)
		close(exe->infd);
	if (exe->outfd > 2)
		close(exe->outfd);
	if (!child)
	{
		while (lst_cmd)
		{
			exe = (t_cmd *) lst_cmd->content;
			if (exe->pipefd > 2)
				close(exe->pipefd);
			lst_cmd = lst_cmd->next;
		}
	}
}

/**
 * @brief	Sets up file descriptors for a command pipeline.
 *
 *			Creates a pipe if exe_next exists, assigns exe->outfd and
 *			exe_next->infd accordingly, and stores the read end in exe.
 *			Closes unnecessary pipe ends to avoid leaks.
 *
 * @param	exe			Command to execute.
 * @param	exe_next	Next command in the pipeline.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on pipe error.
 *
 * @note	Also handles heredoc input by updating exe->infd.
 */
int	exe_setfd(t_cmd *exe, t_cmd *exe_next)
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
