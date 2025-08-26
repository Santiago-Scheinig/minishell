/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_pipe_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/26 18:00:44 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int	ft_waitfor_childs(t_pipex *env, int exit_error)//error to waitpid (*int)
{
	int	i;

	i = -1;
	while (++i < env->cmd_count)
	{
		if (env->waitpid_list[i] > 0)
		{
			if (waitpid(env->waitpid_list[i], NULL, 0) == -1)
			{
				free(env->waitpid_list);
				return (ft_forcend(env, NULL, "Waitpid"));
			}
		}
	}
	free(env->waitpid_list);
	if (exit_error == EXIT_FAILURE)
		return (ft_forcend(env, NULL, "Pipe"));
	return (ft_forcend(env, NULL, NULL));
}

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into a new pipefd[1].
 * @param limitator The string that will work as LIMITATOR.
 * @return Returns the pipefd[0] from where to read everything that was 
 * written on pipefd[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on pipefd[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
int	ft_read_to_limitator(char *limitator)
{
	char	*line;
	int		pipefd[2];
	int		limitator_len;

	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(0);
		limitator_len = ft_strlen(limitator);
		if (line && !ft_strncmp(line, limitator, limitator_len))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

/**
 * Creates a child process to execute a specifc command.
 * @param env The main enviroment pipex structure.
 * @param infd The fd to read from.
 * @param pipefd The pipe fds, [0] to read and [1] to write from.
 * @param envp The main enviroment path.
 * @return Returns the pipefd[0], where the next pipe should read from.
 * @note Every fd is closed in the father and the child process, 
 * excepting the reading pipefd[0] on the father process (since this will 
 * be the return value). This way, if any piped cmd is infinite, it will 
 * SEGPIPE once the dup2 of the infd is no longer being read by the child
 * process.
 */
int	ft_do_fork(t_pipex *env, int infd, int pipefd[2], char **envp)
{
	static int	i;

	env->waitpid_list[i] = fork();
	if (!env->waitpid_list[i] && !(infd < 0))
	{
		if (dup2(infd, STDIN_FILENO) == -1
			|| dup2(pipefd[1], STDOUT_FILENO) == -1)
			perror("Dup2");
		close(infd);
		close(pipefd[0]);
		close(pipefd[1]);
		if (execve(env->cmd[i]->pathname, env->cmd[i]->args, envp) == -1)
			perror("Execve");
	}
	i++;
	if (!(infd < 0))
		close(infd);
	close(pipefd[1]);
	return (pipefd[0]);
}
