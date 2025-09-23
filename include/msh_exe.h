/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/23 14:47:52 by ischeini         ###   ########.fr       */
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
void	fd_endexe(t_cmd *exe, pid_t child);

int		exe_built(t_cmd *exe, t_body *minishell, t_list *envp_lst, char ***envp);

int		setup_pipeline(t_list *cmd_lst);

int		waitcmd(t_body *minishell);
#endif
