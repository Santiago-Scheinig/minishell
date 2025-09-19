/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/19 20:44:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_H
# define MSH_EXE_H

# include "lib/msh_std.h"
# include "lib/msh_tsh.h"
# include <sys/wait.h>
# include <fcntl.h>

/*--------------------------------------------------------------------------*/
/*-----------------------------EXECUTION CHECK------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Verifies the existence of path enviroment.
 * 
 * @param envp The main enviroment where to search for the path enviroment.
 * @return If path exists, returns a CHAR ** with each definitive path specified
 * on it (divided by ':'); If not, returns NULL.
 */
char	**setup_path(const char **envp);

/**
 * Closes all the file descriptors that where used to execute the
 * current pipe iteration.
 * 
 * @param exe A pointer to the cmd to be executed in this current iteration.
 * @param pipefd An array of INT which saves an already initialized pipe().
 */
void	fd_endexe(t_cmd *exe, int pipefd[2]);

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
void	fd_setexe(t_cmd *exe, t_cmd *exe_next, int pipefd[2]);

/**
 * Creates and allocates a STRING with the definitive path to a cmd binary.
 * 
 * @param cmd The name of the command binary to find.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new STRING or NULL if the allocation failed or
 * the cmd can't be access or found as binary on path.
 */
char	*cmd_getpath(char *cmd, char **path);

int		waitcmd(t_body *minishell);
#endif
