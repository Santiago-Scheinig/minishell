/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/20 17:14:23 by sscheini         ###   ########.fr       */
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
void	fd_endexe(t_cmd *exe);

int	setup_pipeline(t_list *cmd_lst);

int		waitcmd(t_body *minishell);
#endif
