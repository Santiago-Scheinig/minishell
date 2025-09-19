/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/19 21:24:26 by sscheini         ###   ########.fr       */
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

	while (1)
	{
		line = get_next_line(0);
		if (!line)//need to print an error of interruption
			break;
		if (line && !ft_strncmp(line, limitator, ft_strlen(limitator)))
		{
			free(line);
			break ;
		}
		write(heredoc[1], line, ft_strlen(line));
		free(line);
	}
	close(heredoc[1]);
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
void	fd_setexe(t_cmd *exe, t_cmd *exe_next, int pipefd[2])
{
	if (exe->fd.exein == -2)
	{
		if (exe->heredoc[0] == -1 && exe->heredoc[1] == -1)
			exe->fd.exein = STDIN_FILENO;
		else
		{
			exe->fd.exein = fd_heredoc(exe->limitator, exe->heredoc);
			exe->heredoc[0] = -1;
			exe->heredoc[1] = -1;
		}
	}
	if (exe->fd.exeout == STDOUT_FILENO)
	{
		exe->fd.exeout = pipefd[1];
		pipefd[1] = 0;
	}
	if (exe_next && exe_next->fd.exein == STDIN_FILENO)
	{
		exe_next->fd.exein = pipefd[0];
		pipefd[0] = 0;
	}
}

/**
 * Closes all the file descriptors that where used to execute the
 * current pipe iteration.
 * 
 * @param exe A pointer to the cmd to be executed in this current iteration.
 * @param pipefd An array of INT which saves an already initialized pipe().
 */
void	fd_endexe(t_cmd *exe, int pipefd[2])
{
	if (pipefd[0])
		close(pipefd[0]);
	if (pipefd[1])
		close(pipefd[1]);
	if (exe->fd.exein > 2)
		close(exe->fd.exein);
	if (exe->fd.exeout > 2)
		close(exe->fd.exeout);
	if (exe->heredoc[0] != -1)
		close(exe->heredoc[0]);
	if (exe->heredoc[1] != -1)
		close(exe->heredoc[1]);
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

/**
 * Creates and allocates a STRING with the definitive path to a cmd binary.
 * 
 * @param cmd The name of the command binary to find.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new STRING or NULL if the allocation failed or
 * the cmd can't be access or found as binary on path.
 */
char	*cmd_getpath(char *cmd, char **path)
{
	char	*cmd_pathname;
	char	*tmp;
	int		i;

	i = -1;
	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);//print no access error!
	}
	while (path[++i] && cmd)
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (NULL);
		cmd_pathname = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!cmd_pathname)
			return (NULL);
		if (!access(cmd_pathname, X_OK))
			return (cmd_pathname);
		free(cmd_pathname);
	}
	return (NULL);//print command not found
}
