/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_bin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/03 17:34:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_BIN_H
# define MSH_BIN_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"

/*--------------------------------------------------------------------------*/
/*---------------------------------BUILT_INS--------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Executes the 'cd' built-in command to change directory.
 *
 *			Updates the current working directory of the shell process.
 *			If no path argument is given, changes to the user's home dir.
 *			On success, updates the 'PWD' variable in the environment list.
 *
 *			Static helper:
 *
 *				- change_pwd():	Finds and updates the 'PWD' variable
 *								in the environment list after a
 *								successful directory change.
 *
 * @param	args	Argument vector from user input. The first element
 *					is "cd", followed by optional directory argument.
 * @param	envp	Double pointer to the environment variable list.
 *
 * @note	If no HOME variable exists, an error is raised.
 * @note	Supports only one argument; additional args cause error.
 * @note	The current directory is changed using chdir(), and on
 *			success, the new path is stored in the "PWD" variable.
 *
 * @return	0 on success, or error code returned by shell_binerr() on
 *			failure.
 */
int			bin_cd(char **args, t_list **envp);

/**
 * @brief	Implements the 'echo' built-in command.
 *
 *			Prints the provided arguments to standard output, separated
 *			by spaces. By default, a trailing newline is printed unless
 *			the '-n' flag is used as the first argument.
 *
 * @param	args	Array of argument strings. The first element is "echo",
 *					and subsequent elements are strings to be printed.
 *
 * @note	Supports the '-n' flag to suppress the trailing newline.
 * @note	If any write() call fails, reports an error through
 *			shell_binerr() and returns the corresponding system code.
 * @note	Each argument is written to STDOUT and separated by a space
 *			unless it is the last one.
 *
 * @return	MSHELL_SUCCESS (0) on success, or system error code if a
 *			write() operation fails.
 */
int			bin_echo(char **args);

/**
 * @brief	Implements the 'env' built-in command.
 *
 *			Prints all current environment variables. The command does
 *			not accept additional arguments or flags other than the
 *			command name itself.
 *
 *			Static helper:
 *				- env_print():	Iterates through envp and prints each
 *								variable line-by-line.
 *
 * @param	args	Array of command arguments. Only args[0] ("env") is
 *					expected; additional entries cause an error.
 * @param	envp	Current environment array of "NAME=VALUE" strings.
 *
 * @note	If an invalid flag or extra argument is provided, an error
 *			is reported using shell_builterr().
 * @note	Returns an error if a write() system call fails during
 *			environment printing.
 *
 * @return	MSHELL_SUCCESS (0) on success, or MSHELL_FAILURE / system
 *			error code on failure.
 */
int			bin_env(char **args, char **envp);

/**
 * @brief	Implements the 'exit' built-in command.
 *
 *			Terminates the shell process, performing a clean shutdown.
 *			If an argument is provided, it is interpreted as the exit
 *			status code. The command may print "exit" when running in
 *			interactive mode.
 *
 * @param	args	Array of arguments. May include one optional numeric
 *					exit code (e.g., "exit 42"). Additional arguments
 *					trigger an error.
 * @param	msh		Pointer to the main shell structure (t_body) used for
 *					cleanup and interactive handling.
 *
 * @note	If running interactively, writes "exit" to STDOUT before
 *			terminating.
 * @note	If more than one argument is passed, returns an error via
 *			shell_builterr() without exiting immediately.
 * @note	If the argument after 'exit' is non-numeric, reports an
 *			INVARGV error and exits with that error code.
 * @note	If no arguments are given, exits with MSHELL_SUCCESS (0).
 *
 * @return	Always returns MSHELL_FAILURE if reached, though the function
 *			normally terminates the process via exit().
 */
int			bin_exit(char **args, t_body *msh);

/**
 * @brief	Implements the 'export' built-in command.
 *
 *			Adds or updates environment variables in the shell's
 *			environment list and array. If no arguments are given,
 *			prints the current environment variables instead.
 *
 *			Static helper:
 *				- get_name():	Extracts variable names from "VAR=VAL"
 *								assignments for update or insertion.
 *
 * @param	argv		Array of strings containing variable assignments or
 *						arguments for export.
 * @param	envp		Double pointer to the current environment array used
 *						by execve() and other commands.
 * @param	lst_envp	Linked list representing environment variables in
 *						the shell context.
 *
 * @note	If a variable already exists, it is updated in-place using
 *			shell_envlst_swp(). Otherwise, shell_envlst_add() inserts it.
 * @note	If called without arguments, it invokes print_lst_t_var() to
 *			display all variables.
 * @note	On allocation or system call failures, an appropriate error is
 *			reported via shell_builterr().
 *
 * @return	MSHELL_SUCCESS on success, or an error code on failure.
 */
int			bin_export(char **argv, char ***envp, t_list *lst_envp);

/**
 * @brief	Implements the 'import' built-in command.
 *
 *			Adds or updates variables from argv into the shell's
 *			environment list and array. Before importing, validates
 *			all entries using check_arguments() to ensure correct
 *			format (VAR=VALUE).
 *
 *			Static helpers:
 *
 *				- get_name():			Extracts variable name from
 *										an assignment string.
 *				- check_arguments():	Validates syntax of import
 *										arguments and removes invalid
 *										entries.
 *
 * @param	argv	Double pointer to the array of assignment strings
 *					to import into the environment.
 * @param	envp	Double pointer to the current environment array
 *					used by execve() and child processes.
 * @param	lst_envp	Linked list representing the shell's
 *					environment variables.
 *
 * @note	Existing variables are updated using shell_envlst_swp(),
 *			while new ones are inserted via shell_envlst_add().
 * @note	Returns MSHELL_FAILURE on syntax errors or allocation
 *			failures.
 *
 * @return	MSHELL_SUCCESS on success, or error code on failure.
 */
int			bin_import(char ***argv, char ***envp, t_list *lst_envp);

/**
 * @brief	Implements the 'pwd' built-in command.
 *
 *			Prints the current working directory to standard output.
 *			If getcwd() fails, retrieves the last known directory from
 *			the environment variable 'PWD' instead.
 *
 * @param	args	Array of command arguments. Supports no options
 *					except rejecting flags starting with '-'.
 * @param	lst_t_var	Linked list of environment variables used
 *					to resolve 'PWD' when getcwd() fails.
 *
 * @note	Uses getcwd() to retrieve the absolute path. If it fails,
 *			the fallback environment value is printed instead.
 * @note	Returns an error through shell_builterr() if a write
 *			or memory allocation fails.
 *
 * @return	0 on success, or an error code if a system call or write
 *			fails.
 */
int			bin_pwd(char **args, t_list *lst_t_var);

/**
 * @brief	Implements the 'unset' built-in command.
 *
 *			Removes one or more variables from the shell's environment
 *			and from the linked list of stored variables.
 *
 *			Static helpers:
 *
 *				- remove_lst():		Frees and removes a node from the
 *									variable list.
 *
 *				- same_name_env():	Checks if an environment string
 *									matches a given variable name.
 *
 *				- check_name():		Searches and removes a matching
 *									variable from both the environment
 *									array and the linked list.
 *
 * @param	arg			Array of variable names to unset. Flags
 *						(e.g., starting with '-') are rejected.
 * @param	envp		Double pointer to the environment array.
 * @param	head_t_var	Double pointer to the head of the t_var
 *						linked list storing environment variables.
 *
 * @note	If a variable exists in both envp and the linked list,
 *			it will be removed from both. The memory of its name and
 *			value is freed.
 * @note	Invalid flags trigger an error through shell_builterr().
 *
 * @return	0 on success, or an error code if input validation or
 *			memory operations fail.
 */
int			bin_unset(char **arg, char ***envp, t_list **head_t_var);

/*--------------------------------------------------------------------------*/
/*-----------------------------------UTILS----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Prints all exported environment variables in the shell.
 *
 *			Iterates over the t_var linked list and prints each
 *			exported variable in the format:
 *				declare -x NAME="VALUE"
 *
 * @param	lst_t_var	Linked list of environment variables.
 *
 * @note	Returns an error via shell_builterr() if any write fails.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on write errors.
 */
int			print_lst_t_var(t_list *lst_t_var);

/**
 * @brief	Validates a string as a valid shell variable name.
 *
 *			The first character must be alphabetic or '_', and the
 *			rest must be alphanumeric or '_'. Stops validation at '='.
 *
 * @param	arg		String to validate.
 * @param	print	If non-zero, prints an error for invalid names.
 *
 * @return	0 if valid, 1 if invalid (or error printed if print=1).
 */
int			is_valid_var(char *arg, int print);
/**
 * @brief	Removes a string from an array of strings.
 *
 *			Frees the string at the specified index and shifts all
 *			subsequent strings one position left. The last element
 *			is set to NULL.
 *
 * @param	arr		Array of strings to modify.
 * @param	index	Position of the string to remove.
 *
 * @return	Pointer to the modified array.
 */
char		**ft_remove_arr(char **arr, int index);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif
