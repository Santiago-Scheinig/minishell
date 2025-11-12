/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:18:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_H
# define MSH_EXE_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

/*--------------------------------------------------------------------------*/
/*---------------------------------STRUCTURES-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Enumeration of built-in shell command types.
 *
 *			Used to identify which built-in command should be executed
 *			by the shell. Each enumerator represents a specific built-in
 *			command or the absence of one.
 */
typedef enum e_bicmd
{
	BINCMD_NOEXE,	// < No built-in command
	BINCMD_CD,		// < 'cd' command
	BINCMD_PWD,		// < 'pwd' command
	BINCMD_ENV,		// < 'env' command
	BINCMD_ECHO,	// < 'echo' command
	BINCMD_IMPORT,	// < 'import' command
	BINCMD_EXPORT,	// < 'export' command
	BINCMD_UNSET,	// < 'unset' command
	BINCMD_EXIT,	// < 'exit' command
}	t_bicmd;

/**
 * @brief	Stores original file descriptors for standard streams.
 *
 *			Used to save the original STDIN, STDOUT, and STDERR
 *			before performing redirections or piping, allowing
 *			restoration after command execution.
 */
typedef struct s_orig
{
	int	orig_stdin;		// < Saved STDIN file descriptor
	int	orig_stdout;	// < Saved STDOUT file descriptor
	int	orig_stderr;	// < Saved STDERR file descriptor
}	t_orig;

/**
 * @brief	Associates a signal number with its descriptive message.
 *
 *			Used for reporting shell errors when a child process
 *			terminates due to a signal. Provides a human-readable
 *			message corresponding to the signal.
 */
typedef struct s_sig_msg
{
	int		sig;	// < Signal number (e.g., SIGSEGV)
	char	*msg;	// < Message describing the signal
}	t_sig_msg;

/*--------------------------------------------------------------------------*/
/*----------------------------EXECUTION LIBRARY-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Main execution function for the minishell.
 *
 *			Handles single built-in commands via exebin_parent(),
 *			or multiple commands via execmd_child(). Sets up signal
 *			handling and PATH array.
 *
 * @param	msh		Shell context with commands, environment, and state.
 *
 * @note	Handles interactive mode signals and cleans up on errors.
 *
 * @return	Exit code of the executed commands, or MSHELL_FAILURE on error.
 */
int			execution(t_body *msh);

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
int			exebin_child(t_cmd *exe, char **envp);

/**
 * @brief	Executes a built-in command in the parent process.
 *
 *			Handles redirection initialization, calls exebin(),
 *			restores original fds, and updates msh->exit_no.
 *
 * @param	bin_no	Identifier of the built-in command.
 * @param	exe		Command structure containing arguments.
 * @param	msh		Shell context containing environment and state.
 *
 * @note	Handles memory allocation errors and forced cleanup.
 *
 * @return	Exit code from the executed command.
 */
int			exebin_parent(int bin_no, t_cmd *exe, t_body *msh);

/**
 * @brief	Executes a list of commands in child processes.
 *
 *			Iterates over lst_cmd, forks children, sets up I/O and
 *			error handling, executes built-ins or external commands,
 *			and frees the PATH array.
 *
 * @param	path		Array of PATH directories.
 * @param	lst_cmd		Linked list of commands to execute.
 * @param	msh			Shell context with environment and state.
 *
 * @return	MSHELL_SUCCESS if execution setup succeeded.
 * @note	Updates msh->err_fd and msh->childs_pid for each command.
 * @note	Handles last command updates via shell_lastcmd_upd().
 */
int			execmd_child(char **path, t_list *lst_cmd, t_body *msh);

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
int			waitexec(t_body *msh);

/*--------------------------------------------------------------------------*/
/*----------------------------------UTILS-----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Closes file descriptors associated with a command or pipeline.
 *
 *			Closes exe->infd and exe->outfd if >2. If child == 0, also
 *			closes pipefds for all commands in the list.
 *
 * @param	lst_cmd	Command or list of commands.
 * @param	child	Flag indicating child process (0 closes all pipefds).
 */
void		exe_endfd(t_list *lst_cmd, pid_t child);

/**
 * @brief	Sets up file descriptors for a command pipeline.
 *
 *			Creates a pipe if exe_next exists, assigns exe->outfd and
 *			exe_next->infd accordingly, and stores the read end in exe.
 *			Closes unnecessary pipe ends to avoid leaks.
 *
 * @param	exe			Command to execute.
 * @param	exe_next	Next command in the pipeline.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on pipe error.
 *
 * @note	Also handles heredoc input by updating exe->infd.
 */
int			exe_setfd(t_cmd *exe, t_cmd *exe_next);

/**
 * @brief	Identifies which built-in command a string represents.
 *
 *			Matches cmd against known built-in commands like export,
 *			cd, env, pwd, echo, unset, exit, or import (assignment).
 *
 * @param	cmd	Command string.
 *
 * @return	Identifier for the built-in, or BINCMD_NOEXE if none.
 */
int			getbin(char *cmd);

/**
 * @brief	Determines if a command corresponds to a known shell.
 *
 *			Checks the basename of cmd against a static list of known
 *			shell names.
 *
 * @param	cmd	Command string to check.
 *
 * @return	true if cmd is a recognized shell, false otherwise.
 */
int			is_shell(const char *cmd);

/**
 * @brief	Checks if a given path is a directory.
 *
 *			Uses stat() to determine if the path exists and is a
 *			directory.
 *
 * @param	path	Path to check.
 *
 * @return	true if path is a directory, false otherwise.
 */
int			is_directory(const char *path);

/*--------------------------------------------------------------------------*/
/*-------------------------------TROUBLESHOOT-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Checks a child process exit status and prints errors.
 *
 *			Handles both normal exit codes and signals. Prints
 *			signal messages (with "core dumped" if applicable)
 *			or forwards exit codes to err_msgfd().
 *
 * @param	status	Status from waitpid().
 * @param	errfd	File descriptor for error messages.
 * @param	inter	Interactive mode flag.
 * @param	line	Input line number for reporting.
 *
 * @return	Exit code for the child process or MSHELL_FAILURE.
 */
int			check_status(int status, int errfd, int inter, int line);

/**
 * @brief	Handles execution errors and terminates the process.
 *
 *			Prints descriptive error messages based on exit_no or
 *			perror() if err_msg is provided. Closes any open fds
 *			from lst_cmd before exiting.
 *
 * @param	exit_no	Exit code indicating type of error.
 * @param	err_msg	Error message prefix, or NULL to use perror().
 * @param	lst_cmd	Command node whose file descriptors should be closed.
 */
int			err_endexe(int exit_no, char *err_msg, t_list *lst_cmd);

/**
 * @brief	Closes all file descriptors in the error array.
 *
 *			Iterates through errfd until -1 is encountered and closes
 *			each fd greater than 2 (STDIN/STDOUT/STDERR are preserved).
 *
 * @param	errfd	Array of file descriptors to close.
 */
void		err_endfd(int *errfd);

/**
 * @brief	Prints error messages read from a file descriptor.
 *
 *			Reads lines from errfd and writes them to STDERR. Handles
 *			different formatting depending on interactive mode and
 *			line numbers.
 *
 * @param	status	Exit status to return.
 * @param	errfd	File descriptor to read error messages from.
 * @param	interactive	Flag indicating interactive shell mode.
 * @param	line	Input line number for formatting.
 *
 * @return	Original status parameter.
 */
int			err_msgfd(int status, int errfd, int interactive, int line);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif