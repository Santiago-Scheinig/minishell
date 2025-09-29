/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/27 15:05:27 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_CMD_H
# define MSH_CMD_H

# include "lib/msh_std.h"
# include "lib/msh_tsh.h"

t_var	*add_env(t_var *head, char *new_env);

void	free_env_list(t_var *env);

void	end_minishell(t_body *minishell);

/**
 * Ensures exported names without '=' exist in envp by adding a trailing '=\0'.
 * 
 * @param args Array of argument strings to process.
 * @param envp Pointer to environment array (address).
 * @param lst Linked list of existing variables.
 * 
 * For each matching variable name without '=', appends "=" to envp via
 * exp_resize.
 * Returns modified args array (some entries may be removed).
 * 
 * @return Modified args pointer, or NULL on allocation failure.
 */
char	**export_no_equal(char **args, char ***envp, t_list *lst);

/**
 * Processes and validates export arguments; returns normalized args.
 * 
 * @param args Array of argument strings to export.
 * @param head Linked list head of current variables.
 * @param envp Pointer to environment array pointer (used by helpers).
 * 
 * Returns a possibly modified args array (duplicates removed, errors reported).
 * If no args provided, prints export list and returns NULL.
 * 
 * @return Adjusted args array or NULL.
 */
char	**ft_isal_num(char **args, t_list *head, char ***envp);

/**
 * Removes an element from a NULL-terminated array of strings.
 * 
 * @param arr Array of strings to modify.
 * @param index Index of the element to remove.
 * 
 * Frees the removed element and shifts remaining pointers down; sets final
 * slot to NULL.
 * 
 * @return The modified array pointer.
 */
char	**ft_remove_arr(char **arr, int index);

/**
 * Removes duplicate/newer entries among the given args list.
 * 
 * @param args Array of argument strings.
 * 
 * Compares names up to '=' and removes duplicates preferring entries with '='.
 * 
 * @return Modified args array.
 */
char	**export_no_dup(char **args);

/**
 * @param env_lst Linked list of t_var nodes containing name, value and
 * exported flag.
 * 
 * Iterates the list and writes each exported variable in the format:
 *   declare -x NAME="VALUE"
 * or just "declare -x NAME" if no value.
 */
void	print_export(t_list *env_lst);

/**
 * Prints the environment array (envp) to stdout, one per line.
 * 
 * @param envp Null-terminated array of environment strings ("NAME=VALUE").
 * 
 * Writes each env string followed by a newline.
 */
void	print_env(char **envp);

/**
 * Attempts to change the stored value of an existing variable.
 * 
 * @param aux Pointer to the t_var node representing the variable.
 * @param envp Pointer-to-pointer to the envp array used for string updates.
 * @param new_env Array with new_env[0] equal to the new "NAME=VALUE" string.
 * @param export Flag indicating export behavior.
 * 
 * Matches names and updates export flag or value when appropriate.
 * 
 * @return 0 on handled change, 1 if sizes differ, or non-zero on error.
 */
int		change_value_env(t_var *aux, char ***envp, char **new_env, int export);

/**
 * Sets variable value in env array and in var struct when '=' present.
 * 
 * @param aux Pointer to t_var representing the variable in list.
 * @param envp Environment array.
 * @param sign Pointer to '=' character inside new_env string (or NULL).
 * @param new_env New environment string "NAME=VALUE".
 * 
 * Replaces envp entry and updates aux->value when appropriate.
 * 
 * @return 0 on success, non-zero if a system error was reported.
 */
int		set_equal(t_var *aux, char **envp, char *sign, char *new_env);

/**
 * Appends new variables (not previously in list) to the variable list.
 * 
 * @param new_env Array of strings "NAME" or "NAME=VALUE" for new entries.
 * @param head Linked list head where new nodes will be appended.
 * @param export Export flag to set on created nodes.
 * 
 * Allocates nodes and appends them to the list tail.
 * 
 * @return 0 on success, non-zero on failure.
 */
int		new_envp(char **new_env, t_list **head, int export);

/**
 * Resizes the environment array to append new entries.
 * 
 * @param args Array of new strings to append (args_len treated inside).
 * @param envp Pointer to environment array to resize (address).
 * 
 * Reallocates envp to fit extra entries, copies them and updates pointer.
 * Prints and deduplicates resulting envp.
 * 
 * @return 0 on success, non-zero on error.
 */
int		exp_resize(char **args, char ***envp);

/**
 * Validates that the provided identifier is suitable for export.
 * 
 * @param arg Candidate string like "NAME" or "NAME=VALUE".
 * 
 * Ensures first char is alphabetic or '_' and subsequent chars are
 * alphanumeric or '_'.
 * Prints an error via built_end on invalid identifiers.
 * 
 * @return 0 if valid, non-zero error code otherwise.
 */
int		is_valid_identifier(char *arg);

/*************************************************************************** */
/**
 * BUILT IN COMANDS
 */

/**
 * Handles error messages for built-in shell commands and print stderr.
 * Depending on the error type and flags, it formats and outputs the
 * appropriate message.
 * 
 * @param name The name of the command that caused the error.
 * @param type A string indicating the type of error to handle.
 * @param flags Additional flags or arguments related to the error.
 * @param error A character representing a specific invalid flag option
 * @return Returns 2 for invalid flag errors, otherwise returns 1.
 * 
 * @note Prints error messages prefixed with "minishell:".
 * @note Supports various error types such as argument count, Invalid flags,
 * missing HOME, System errors, Invalid identifiers, and Numeric argument
 * errors.
 */
int		built_end(char *name, char *type, char *flags, char error);

/**
 * Exits the shell process with an optional numeric status.
 * 
 * @param args Array of arguments passed to exit (args[0] is "exit").
 * @param minishell Pointer to the shell context; used for interactive behavior and cleanup.
 * 
 * If running interactively prints "exit". Validates argument count and numeric argument.
 * 
 * @return void (function exits the process on success or error cases).
 * @note - If more than one numeric argument is provided, prints error and does not exit.
 * - Performs cleanup/forced end via forcend() when needed.
 */
void	msh_exit(char **args, t_body *minishell);

/**
 * Prints arguments to standard output, separated by spaces.
 * 
 * @param args Array of arguments passed to the echo command
 * (args[0] is "echo").
 * 
 * @note - If any leading argument(s) are "-n", no trailing newline is printed.
 */
void	msh_echo(char **args);

/**
 * Built-in 'unset' command implementation for msh.
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param env_lst Pointer-to-list head of environment variables.
 * @param arg Array of variable names to unset.
 * 
 * Validates flags, then removes each matching variable from both envp and the
 * list.
 * 
 * @return 0 on success, non-zero on error.
 */
int		msh_unset(char ***envp, t_list **env_lst, char **arg);

/**
 * Imports variables into envp/list from argument list (used for PS1/initial import).
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param head Pointer-to-list head of current variables.
 * @param args Array of argument strings to import ("NAME" or "NAME=VALUE").
 * 
 * Validates identifiers, updates existing entries, and appends new ones.
 * 
 * @return 0 on success, non-zero on failure.
 */
int		msh_import(char ***envp, t_list **head, char **args);

/**
 * Built-in 'export' command handler.
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param head Pointer-to-list head of current variables.
 * @param args Arguments for export command.
 * 
 * Validates, updates existing variables, resizes envp and appends new vars.
 * 
 * @return 0 on success, non-zero on failure.
 */
int		msh_export(char ***envp, t_list **head, char **args);

/**
 * Built-in 'cd' command for msh.
 * 
 * @param args Argument array for cd (args[0] is "cd").
 * @param envp Pointer-to-list head of environment variables
 * (used to update PWD).
 * 
 * Changes the working directory to the specified path or HOME if no arg
 * provided.
 * Reports errors and returns corresponding error.
 * 
 * @return 0 on success, non-zero on error.
 * @note - Print errors directly.
 */
int		msh_cd(char **args, t_list **envp);

/**
 * Built-in 'pwd' command handler.
 * 
 * @param args Argument array for pwd (args[0] is "pwd").
 * @param envp Linked list or array representing environment variables.
 * 
 * Prints the current working directory. If getcwd fails, attempts to read
 * PWD from env.
 * 
 * @return 0 on success, non-zero on error.
 */
int		msh_pwd(char **args, t_list *envp);

/**
 * Built-in 'env' command handler.
 * 
 * @param args Command arguments array (args[0] == "env").
 * @param envp Environment array to print.
 * 
 * Validates flags and argument count, then prints environment.
 * 
 * @return 0 on success, non-zero on error.
 */
int		msh_env(char **args, char **envp);


#endif
