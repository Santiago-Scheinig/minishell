/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:33:37 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_H
# define MSH_EXE_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"
# include <sys/wait.h>
# include <fcntl.h>

/*--------------------------------------------------------------------------*/
/*-----------------------------EXECUTION CHECK------------------------------*/
/*--------------------------------------------------------------------------*/

typedef enum e_bicmd
{
	BINCMD_NOEXE,
	BINCMD_CD,
	BINCMD_PWD,
	BINCMD_ENV,
	BINCMD_ECHO,
	BINCMD_IMPORT,
	BINCMD_EXPORT,
	BINCMD_UNSET,
	BINCMD_EXIT,
}	t_bicmd;

typedef struct s_orig
{
	int	orig_stdin;
	int	orig_stdout;
	int	orig_stderr;
}	t_orig;

typedef struct s_sig_msg
{
	int		sig;
	char	*msg;
}	t_sig_msg;

int	getbin(char *cmd);

int	err_msgfd(int status, int errfd, int interactive, int line);

int	err_endexe(int exit_no, char *err_msg, t_list *lst_cmd);

int	is_shell(const char *cmd);

void	exe_endfd(t_list *lst_cmd, pid_t child);

void	err_endfd(int *errfd);

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
int	exebin_child(t_cmd *exe, char **envp);

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

int	execution(t_body *msh);

/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * 
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int	waitexec(t_body *msh);

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
int	exe_setfd(t_cmd *exe, t_cmd *exe_next);

int	exebin_parent(int bin_no, t_cmd *exe, t_body *msh);

int	execmd_child(char **path, t_list *lst_cmd, t_body *msh);

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

int		exend(int exit_no, char *err_msg, t_list *cmd_lst);

int		waitcmd(t_body *minishell);

#endif
