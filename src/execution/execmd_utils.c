/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/25 14:36:21 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

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
static int	fd_setexe(t_cmd *exe, t_cmd *exe_next)
{
	int pipefd[2];

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

void	fd_endexe(t_list *cmd_lst, pid_t child)
{
	t_cmd *exe;

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

int		setup_pipeline(t_list *cmd_lst)
{
	t_list	*aux;
	t_cmd	*exe;
	t_cmd	*exe_next;
	int		i;

	i = -1;
	aux = cmd_lst;
	while (cmd_lst)
	{
		exe = cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = cmd_lst->next->content;
		if (fd_setexe(exe, exe_next))
		{
			fd_endexe(aux, 1);
			return(redirend(NULL, MSHELL_FAILURE));
		}
		cmd_lst = cmd_lst->next;
	}
	return(MSHELL_SUCCESS);
}

/**
 * Verifies the existence of path enviroment.
 * 
 * @param envp The main enviroment where to search for the path enviroment.
 * @return If path exists, returns a CHAR ** with each definitive path specified
 * on it (divided by ':'); If not, returns NULL.
 */
char	**setup_path(const char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp("PATH=", envp[i], 5))
			return (ft_split(&envp[i][5], ':'));
	return (NULL);
}
