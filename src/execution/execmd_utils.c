/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/21 16:06:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into heredoc[1].
 * 
 * @param limitator The string that will work as LIMITATOR.
 * @param heredoc An array of INT which saves an already initialized pipe()
 * @return Returns heredoc[0] from where to read everything that was 
 * written on heredoc[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on heredoc[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
static int	fd_heredoc(char *limitator, int heredoc[2])
{
	char	*line;
	const char	*errmsg = "here-document at line 1 delimited by end-of-file";

	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			if (errno == ENOMEM)//forcend should check on errno instead of following msg to know what to print?
				return (-1);//i also need to verify if read failed and print a message accordingly
			ft_printfd(2, "\nmsh: warning: %s (wanted '%s')\n", errmsg, limitator);
			break;
		}
		if (line && !ft_strncmp(line, limitator, ft_strlen(limitator)))
		{
			free(line);
			break;
		}
		write(heredoc[1], line, ft_strlen(line));//write can fail! line x where x is the line its writing of heredoc
		free(line);
	}
	close(heredoc[1]);//How could i add the heredoc to the history??
	return (heredoc[0]);
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
static int	fd_setexe(t_cmd *exe, t_cmd *exe_next)
{
	int pipefd[2];

	if (exe->fd.exein == -2)
	{
		exe->fd.exein = fd_heredoc(exe->limitator, exe->heredoc);
		if (exe->fd.exein == -1)
			return (MSHELL_FAILURE);
		exe->heredoc[0] = -1;
		exe->heredoc[1] = -1;
	}
	if (exe_next)
	{
		if (pipe(pipefd))
			return (MSHELL_FAILURE);
		if (exe->fd.exeout == STDOUT_FILENO)
			exe->fd.exeout = pipefd[1];
		else
			close(pipefd[1]);
		if (exe_next->fd.exein == STDIN_FILENO)
			exe_next->fd.exein = pipefd[0];
		else
			close(pipefd[0]);
	}
	return (MSHELL_SUCCESS);
}

void	fd_endexe(t_cmd *exe)
{
	if (exe->fd.exein > 2)
		close(exe->fd.exein);
	if (exe->fd.exeout > 2)
		close(exe->fd.exeout);
}

int		setup_pipeline(t_list *cmd_lst)
{
	t_cmd	*exe;
	t_cmd	*exe_next;
	int		i;

	i = -1;
	while (cmd_lst)
	{
		exe = cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = cmd_lst->next->content;
		if (fd_setexe(exe, exe_next))
			return(MSHELL_FAILURE);//print a broken pipeline, but do not end minishell. The function should also close all fds opened.
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

