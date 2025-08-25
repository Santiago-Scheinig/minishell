/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_pipe_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/06/04 14:16:17 by sscheini         ###   ########.fr       */
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

/**
 * Executes and pipes every command specified on the program enviroment.
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
int	ft_do_pipe(t_pipex *env, char **envp)
{
	int	i;
	int	infd;
	int	pipefd[2];

	i = -1;
	infd = -1;
	env->waitpid_list = ft_calloc((env->cmd_count), sizeof(pid_t));
	if (!env->waitpid_list)
		return (-1);
	while (++i < env->cmd_count)
	{
		if (!i && env->infile)
			infd = open(env->infile, O_RDONLY);
		if (pipe(pipefd) == -1)
			return (ft_waitfor_childs(env, EXIT_FAILURE));
		if (i == env->cmd_count - 1)
		{
			close(pipefd[1]);
			pipefd[1] = open(env->outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (pipefd[1] < 0)
				return (ft_waitfor_childs(env, EXIT_FAILURE));
		}
		infd = ft_do_fork(env, infd, pipefd, envp);
	}
	return (ft_waitfor_childs(env, EXIT_SUCCESS));
}
