/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/05 16:26:10 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (false);
	if (S_ISDIR(path_stat.st_mode))
		return (true);
	return (false);
}

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
 * Connects all the file descriptors between the cmd to execute
 * and the following cmd.
 * 
 * @param exe A pointer to the cmd to be executed in this current iteration.
 * @param exe_next A pointer to the next cmd to be executed.
 * @param pipefd An array of INT which saves an already initialized pipe().
 * @note Also handles heredoc redirections, saving the reading end of that
 * pipe as the [exe] infile, and closing the previous one.
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
