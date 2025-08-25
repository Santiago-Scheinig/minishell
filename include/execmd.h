/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 17:42:50 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECMD_H
# define EXECMD_H

# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include "minishell.h"
# include "libft.h"

/*--------------------------------------------------------------------------*/
/*---------------------------------FAILSAFE---------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Pipex failsafe, in case of error, frees all memory that could remain
 * allocated in the main structure.
 * @param env The main environment pipex structure.
 * @param path The enviroment path where execve searches for binaries.
 * @param ft_error The function name where the error in question occured.
 */
int		ft_forcend(t_pipex *env, char **path, char *ft_error);

/*--------------------------------------------------------------------------*/
/*--------------------------------PIPE_UTILS--------------------------------*/
/*--------------------------------------------------------------------------*/

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
int		ft_do_fork(t_pipex *env, int infd, int pipefd[2], char **envp);

/**
 * Executes and pipes every command specified on the program enviroment,
 * but the first command will read from the STDIN instead of the infile 
 * (until the LIMITATOR is writen).
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
int		ft_do_here_doc(t_pipex *env, char **envp, char *limitator);

/**
 * Executes and pipes every command specified on the program enviroment.
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
int		ft_do_pipe(t_pipex *env, char **envp);

/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int		ft_waitfor_childs(t_pipex *env, int exit_error);

/**
 * Verifies the existence of path enviroment.
 * @param envp The main enviroment where to search for the path enviroment.
 * @return If path exists, returns a CHAR ** with each definitive path
 * specified on it (divided by ':'); If not, returns NULL.
 */
char	**ft_check_path(const char **envp);

/**
 * Creates and allocates a CMD strucure.
 * @param cmd The name of the command.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new CMD structure or NULL if the allocation
 * failed or the cmd can't be found as binary on path.
 */
t_cmd	*ft_new_cmd(const char *cmd, const char **path);

/**
 * Returns the amount of comands on the main arguments, following the format
 * specified for pipex.
 * @param argv The main arguments.
 */
int		ft_get_cmd_count(char **argv);

#endif
