/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:09:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_H
# define MSH_EXE_H

# include "lib/msh_std.h"
# include <sys/wait.h>
# include <fcntl.h>

/*--------------------------------------------------------------------------*/
/*-----------------------------EXECUTION CHECK------------------------------*/
/*--------------------------------------------------------------------------*/

typedef struct s_sig_msg
{
	int		sig;
	char	*msg;
}	t_sig_msg;

/**
 * Verifies the existence of path enviroment.
 * 
 * @param envp The main enviroment where to search for the path enviroment.
 * @return If path exists, returns a CHAR ** with each definitive path specified
 * on it (divided by ':'); If not, returns NULL.
 */
char	**setup_path(const char **envp);

void	fd_endexe(t_list *cmd_lst, pid_t child);

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param exe Pointer to the struct of commands.
 * @param minishell Pointer to the main shell structure containing environment
 * variables.
 * @param envp_lst pointer to the struct of exported enviroments.
 * @param envp A linked list node containing environment variable data.
 * @return Returns a -1 if there is not execution inside, 0 if the executed
 * comand works, and 1 or 2 in case an error inside of the commands.
 */
int		father_bicmd(t_cmd *exe, t_body *minishell);

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param cmds An array of the comands to execute.
 * @param envp A linked list node containing environment variable data.
 * @return Returns a -1 if there is not execution inside, else exit with 0 if
 * the executed comand works, and 1 or 2 in case an error inside of the
 * commands.
 */
int		child_bicmd(t_cmd *exe, char **envp);

int		setup_pipeline(t_list *cmd_lst);

int		exend(int exit_no, char *err_msg, t_list *cmd_lst);

int		waitcmd(t_body *minishell);
#endif
