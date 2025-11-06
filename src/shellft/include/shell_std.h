/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_std.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:39:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 11:17:56 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_STD_H
# define SHELL_STD_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "libft.h"
# include "libft_utils.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <termios.h>
# include <signal.h>
# include <errno.h>

/*--------------------------------------------------------------------------*/
/*--------------------------------SHELL GLOBAL------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief Global flag indicating an interrupt signal was received.
 *
 * This variable is set to 1 by the SIGINT handler (set_flag) when the
 * shell is interrupted (e.g., Ctrl+C) in non-interactive mode. Other
 * parts of the shell check this flag to perform cleanup or abort
 * processing safely.
 *
 * @note	Declared volatile to ensure correct behavior in signal handlers.
 * @note	Declared sig_atomic_t to guarantee atomic reads/writes in
 *			signal handlers.
 */
extern volatile sig_atomic_t	g_signal_received;

/*--------------------------------------------------------------------------*/
/*------------------------------SHELL ENUMERATIONS--------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief Enumeration of token types used during lexical analysis.
 *
 * Represents the type of each parsed token in the shell input.
 */
typedef enum e_token_type
{
	WORD,			// Command or argument
	PIPE,			// Pipe symbol '|', used to connect commands
	REDIR_IN,		// Input redirection '<'
	REDIR_OUT,		// Output redirection '>'
	REDIR_APPEND,	// Output append redirection '>>'
	HEREDOC,		// Here-document input '<<'
	END,			// End of line or input
}	t_token_type;

/**
 * @brief Enumeration of possible shell error codes.
 *
 * Represents the various outcomes of shell operations, including success,
 * failure, misusage, command execution errors, and signal handler issues.
 */
typedef enum e_error
{
	MSHELL_FATAL = -1,		// Fatal error, unrecoverable
	MSHELL_SUCCESS,			// Operation succeeded
	MSHELL_FAILURE,			// General failure
	MSHELL_MISSUSE,			// Incorrect usage of shell or function
	MSHELL_CMD_INVEXE = 126,// Command invoked cannot execute
	MSHELL_CMD_NOTEXE,		// Command not found or not executable
	MSHELL_CMD_ISDIR,		// Command is a directory
	MSHELL_SIG_HANDLR,		// Error in setting up signal handler
}	t_error;

typedef enum e_binerr
{
	SYSFAIL = -1,	//Fatal system failure
	INVARGC,		//Invalid number of arguments
	INVFLGS,		//Invalid flag option
	INVHOME,		//Invalid HOME, doesn't exist on envp
	INVIDFY,		//Invalid identifier
	INVARGV,		//Invalid argument
}	t_binerr;

/*--------------------------------------------------------------------------*/
/*------------------------------SHELL STRUCTURES----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief Main shell state structure.
 *
 * Holds all information about the current state of the minishell,
 * including terminal settings, environment variables, input, commands,
 * child process tracking, and tokenized input.
 *
 * @note	< err_fd is allocated and owned by the shell to handle child
 *			process errors. Freed during cleanup. >?
 * @note	<> exit_ft stores the function name that caused the last failure
 *			in forcend().
 * @note	<> childs_pid is allocated and freed on cleanup, tracking all
 *			child process PIDs.
 * @note	<> lst_t_cmd, lst_t_var, lst_t_token are linked lists of commands,
 *			environment variables, and tokens, respectively. Each node's
 *			content must be cast to the appropriate type.
 */
typedef struct s_body
{
	struct termios	orig_term;	// < Original terminal settings
	int				interactive;// < If shell runs interactively, otherwise 0
	int				line;		// < Current input line number or count
	int				exit_no;	// < Last command's exit code
	int				input_result;// < Result of the last input processing
	int				*err_fd;	// < Pipe fds for child error handling
	const char		*exit_ft;	// < Function name causing last failure
	char			**envp;		// < Array of environment strings
	pid_t			*childs_pid;// < Array of child PIDs
	t_list			*head_cmd;	// < Linked list of t_cmd commands
	t_list			*head_envar;// < Linked list of t_var environment variables
	t_list			*head_token;//< Linked list of t_token parsed tokens
}	t_body;

/**
 * @brief Represents a parsed command in the shell.
 *
 * Stores information about the command's executable path, arguments,
 * and I/O configuration. Each command is intended to be stored in a
 * linked list within the shell state (t_body->lst_t_cmd).
 *
 * @note	Memory for pathname, argv, must be managed carefully.
 */
typedef struct s_cmd
{
	char	*pathname;	// Path to the executable or builtin command
	char	**argv;		// Array of argument strings, command name at argv[0]
	int		infd;		// Input file descriptor (0 for stdin or file/pipe)
	int		outfd;		// Output file descriptor (1 for stdout or file/pipe)
	int		pipefd;		// Write end of a pipe if part of a pipeline, else -1
}	t_cmd;

/**
 * @brief Represents a parsed token in the shell input.
 *
 * Stores the raw string of the token, a character mask, its type, and
 * heredoc information if applicable.
 */
typedef struct s_token
{
	char			*str;		// Raw token string
	char			*mask;		// Character mask corresponding to str
	int				heredoc_fd;	// File descriptor of heredoc, 0 if none
	t_token_type	type;		// Type of token (WORD, PIPE, REDIR_IN, etc.)
}	t_token;

/**
 * @brief Represents an environment variable in the shell.
 *
 * Stores the name, value, and export status of an environment variable.
 * Used in the linked list t_body->lst_t_var.
 */
typedef struct s_var
{
	char	*value;		// Value of the environment variable
	char	*name;		// Name of the environment variable
	int		exported;	// 1 if exported to child processes, 0 otherwise
}	t_var;

/*--------------------------------------------------------------------------*/
/*--------------------------------SHELL LIBRARY-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Adds a new environment variable to the envp array.
 *
 *			Reallocates the envp array to hold one additional variable,
 *			copies the new variable string, and ensures the array remains
 *			NULL-terminated.
 *
 * @param	var		Pointer to the string representing the environment variable
 *					to add.
 * @param	envp	Double pointer to the environment array to modify.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for freeing the envp array and its
 *			strings.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int			shell_envarr_add(char *var, char ***envp);

/**
 * @brief	Replaces the value of an existing environment variable.
 *
 *			Searches the envp array for a variable with the same name as var.
 *			If found, swaps its value with the new one. Memory for temporary
 *			names is allocated and freed internally.
 *
 * @param	var		Pointer to the string containing the new variable value.
 * @param	envp	Double pointer to the environment array to modify.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for managing the envp array.
 *
 * @return	MSHELL_SUCCESS on successful swap, MSHELL_FAILURE otherwise.
 */
int			shell_envarr_swp(char *var, char ***envp);

/**
 * @brief	Searches the environment list for a variable and returns its value.
 *
 *			Sorts the environment list using shell_sortenv() before searching.
 *			Compares each variable name with the given one. If a match is found
 *			and the variable has a non-empty value, that value is returned.
 *			If not found or empty, returns NULL.
 *
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 * @param	name		Name of the environment variable to search for.
 *
 * @note	Returns NULL if lst_envp is empty, the variable is not found,
 *			or its value string is empty.
 * @note	The returned pointer is part of the internal list and should not
 *			be freed manually.
 *
 * @return	Pointer to the variable's value string, or NULL if not found or
 *			value is empty.
 */
char		*shell_envchr(int *exported, const char *name, t_list *lst_envp);

/**
 * @brief	Duplicates the environment variable array and updates SHLVL.
 *
 *			Allocates a new NULL-terminated copy of the given envp array.
 *			Each string is duplicated with ft_strdup(). When "SHLVL=" is
 *			found, its value is incremented using change_shlvl(). If any
 *			allocation fails, all previously allocated memory is freed.
 *
 * @param	envp	Pointer to the original environment variable array.
 *
 * @note	Returns NULL if allocation fails or if envp is invalid.
 * @note	The returned array and its contents must be freed by the caller.
 *
 * @return	Newly allocated environment array with updated SHLVL, or NULL
 *			on allocation failure.
 */
char		**shell_envdup(const char **envp);

/**
 * @brief	Duplicates an environment array into a linked list of t_var nodes.
 *
 *			Iterates through the envp array, creating a new t_var with
 *			shell_envnew() for each entry. Each t_var is wrapped in a list
 *			node and appended to the new linked list. If any allocation fails,
 *			all previously allocated nodes are freed.
 *
 * @param	envp	Pointer to the original NULL-terminated environment array.
 *
 * @note	Returns NULL if allocation fails at any point.
 * @note	The returned list and its t_var contents must be freed with
 *			shell_lstclear() and shell_delenv().
 *
 * @return	Pointer to the newly created linked list of t_var nodes, or NULL
 *			on error.
 */
t_list		*shell_envdup_lst(char **envp);

/**
 * @brief	Initializes core environment variables required by minishell.
 *
 *			Duplicates the original environment into both array and list
 *			formats, then ensures essential variables (PS1, PWD, SHLVL, "_")
 *			are present. Missing entries are created with default values,
 *			while existing ones are synchronized or updated as needed.
 *
 * @param	envp_src	Original environment variable array from main().
 * @param	envp_des	Pointer to store the duplicated environment array.
 * @param	envp_head	Pointer to store the linked list of t_var entries.
 *
 * @note	PS1 is set to "\\u:\\w\\$ " if missing.
 * @note	PWD is updated to match the current working directory.
 * @note	SHLVL defaults to 1 when absent, tracking shell nesting depth.
 * @note	The "_" variable references the last executed command.
 *
 * @return	MSHELL_SUCCESS if initialization completes successfully, or
 *			MSHELL_FAILURE if any setup step fails.
 */
int			shell_envini(char **env_src, char ***env_des, t_list **env_head);

/**
 * @brief	Adds a new environment variable to a linked list and optionally
 * 			envp.
 *
 *			Creates a new t_var from var and appends it to the linked list.
 *			If exp is true, also adds the variable to the envp array. Frees
 *			memory on allocation failure.
 *
 * @param	e			Flag indicating whether the variable should be
 * 						exported.
 * @param	var			String representing the new environment variable
 * 						(NAME=VALUE).
 * @param	envp		Double pointer to the environment array for exported
 * 						variables.
 * @param	envp_lst	Pointer to the linked list of environment variables.
 *
 * @note	Returns MSHELL_FAILURE if memory allocation fails.
 * @note	The caller is responsible for managing the linked list and envp.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int			shell_envlst_add(int e, char *var, char ***envp, t_list *lst_envp);

/**
 * @brief	Updates the value and exported flag of an environment variable.
 *
 *			Searches the linked list of t_var for a variable matching the
 *			name in var. If found, swaps its value and updates the exported
 *			flag. If exported is true, adds the variable to the envp array.
 *
 * @param	e			New exported flag to set for the variable.
 * @param	var			String containing the variable name and new value
 * 						(NAME=VALUE).
 * @param	envp		Double pointer to the environment array for addition
 * 						if exported.
 * @param	envp_lst	Pointer to the linked list of environment variables.
 *
 * @note	Returns MSHELL_FAILURE if var is NULL, memory allocation fails,
 *			or swap_value fails.
 * @note	The caller is responsible for managing the envp array and linked
 * 			list.
 *
 * @return	MSHELL_SUCCESS on successful update, MSHELL_FAILURE otherwise.
 */
int			shell_envlst_swp(int e, char *var, char ***envp, t_list *lst_envp);

/**
 * @brief	Sorts a linked list of environment variables alphabetically.
 *
 *			Iteratively compares adjacent t_var elements in the list and
 *			swaps them using swap_env() if their names are out of order.
 *			Sorting continues until a full pass is made with no swaps,
 *			ensuring all variables are ordered lexicographically.
 *
 * @param	hlst_envp	Pointer to the head of the t_list environment list.
 *
 * @note	Uses ft_strncmp() for name comparison based on string length.
 * @note	Sorting is done in place; no new list is allocated.
 * @note	Requires that each node's content is a valid t_var structure.
 */
void		shell_envsort(t_list **hlst_envp);

/**
 * REWRITE
 */
int			shell_lstadd_newtkn(int i, char *str, char *base, t_list *lst_tkn);

int			shell_lastcmd_upd(char *cmd, char ***envp, t_list *head_envp);

/**
 * @brief	Moves memory from src to dest with a mask, safely handling overlap.
 *
 *			Performs a memory copy of n bytes from src to dest. If dest is
 *			higher in memory than src, calls iq_movedn() to copy backward.
 *			Otherwise, calls iq_moveup() to copy forward. Each byte is
 *			controlled by the corresponding entry in the mask array.
 *
 * @param	dest	Pointer to the destination memory buffer.
 * @param	src		Pointer to the source memory buffer.
 * @param	mask	Pointer to a mask array controlling which bytes are copied.
 * @param	n		Number of bytes to move.
 *
 * @note	If both dest and src are NULL, returns dest without performing
 *			any operation.
 * @note	Mask array should be at least n bytes long to avoid undefined
 *			behavior.
 *
 * @return	Pointer to the destination memory buffer (dest).
 */
void		*shell_memmove(void *dest, void *src, void *mask, size_t n);

/**
 * @brief	Creates a new t_var environment variable from a raw string.
 *
 *			Parses the given envp string to separate the variable name and
 *			value, then calls set_envar() to allocate and initialize a new
 *			t_var structure. The exported flag is assigned accordingly.
 *
 * @param	envp		Pointer to the environment string (e.g., "USER=root").
 * @param	exported	Boolean flag indicating if the variable is exported.
 *
 * @note	Returns NULL if allocation fails or envp is invalid.
 * @note	The returned structure must be freed with shell_delenv() when
 *			no longer needed.
 *
 * @return	Pointer to the newly created t_var structure, or NULL on error.
 */
t_var		*shell_newenv(const char *envp, int exported);

/**
 * @brief	Generates the shell prompt string based on PS1 and current path.
 *
 *			Retrieves the PS1 environment variable and the current working
 *			directory. Calls pwd_name() to construct the final prompt string.
 *			If PS1 is not set, uses an empty string as default. Allocates a
 *			new string which must be freed by the caller.
 *
 * @param	lst_envp	Pointer to the list of environment variables (t_list).
 *
 * @note	Returns NULL if memory allocation fails at any stage.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Pointer to the newly allocated prompt string, or NULL on error.
 */
char		*shell_newpmt(t_list *lst_envp);

/**
 * REWRITE
 */
t_token		*shell_newtkn(char *str, char *base);

/**
 * @brief	Splits a shell command string into words and operators.
 *
 *			Counts the number of operators and words in the input string,
 *			then allocates an array of strings. Uses create_split() to fill
 *			the array with the individual words and operator tokens.
 *
 * @param	str	Pointer to the input command string to split.
 *
 * @note	Returns an empty array if the input string is empty.
 * @note	Returns NULL if memory allocation fails.
 * @note	The returned array and its strings must be freed by the caller.
 *
 * @return	Pointer to the newly allocated array of strings, or NULL on error.
 */
char		**shell_split(const char *str);

/*--------------------------------------------------------------------------*/
/*-------------------------------SHELL HANDLERS-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Restores default handling for SIGINT and SIGQUIT signals.
 *
 *			Sets up a sigaction structure with SIG_DFL as the handler and an
 *			empty signal mask. Calls sigaction() for both SIGINT and SIGQUIT
 *			to restore their default behavior.
 *
 * @note	Use this when the shell needs standard signal behavior, e.g.,
 *			before executing child processes.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if any sigaction call
 * 			fails.
 */
int			shell_sigdfl_all(void);

/**
 * @brief	Ignores all common interactive signals (SIGINT and SIGQUIT).
 *
 *			Sets up a sigaction structure with SIG_IGN as the handler for
 *			SIGINT. Adds both SIGINT and SIGQUIT to the signal mask and
 *			sets the SA_RESTART flag. Calls sigaction() to apply the settings.
 *
 * @note	Prevents the shell from being interrupted or quit by Ctrl+C or
 *			Ctrl+\ during critical operations.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if sigaction fails.
 */
int			shell_sigign_all(void);

/**
 * @brief	Handles SIGINT signal during shell input reading.
 *
 * 			Checks if a SIGINT (Ctrl+C) was received by inspecting the
 * 			global flag g_signal_received. If a signal was received, the
 * 			flag is reset and, for non-interactive shells, forces the
 * 			shell to terminate with the stored exit code.
 *
 * @param	msh	Pointer to the shell state structure t_body.
 *
 * @note	The function does not terminate the shell if it is in interactive
 *			mode; it only resets the signal flag.
 *
 * @return	MSHELL_SIG_HANDLR constant indicating a handled SIGINT.
 */
int			shell_sigint_read(t_body *msh);

/**
 * @brief	Sets up the SIGINT handler for the shell.
 *
 * 			Configures a sigaction structure to handle SIGINT depending
 * 			on whether the shell is in interactive mode. Blocks SIGQUIT
 * 			and SIGINT during handler execution, and uses SA_RESTART
 * 			to automatically restart interrupted system calls.
 *
 * @param	interactive	Integer flag indicating shell mode:
 *						1 for interactive, 0 for non-interactive.
 *
 * @note	Interactive mode uses sigdlr_newpmt handler to refresh prompt.
 * @note	Non-interactive mode uses sigdlr_setflg handler to set signal flag.
 *
 * @return	MSHELL_SUCCESS on successful registration, MSHELL_FAILURE if
 *			sigaction fails.
 */
int			shell_sigint(int interactive);

/**
 * @brief	Configures the shell to ignore the SIGQUIT signal (Ctrl+\).
 *
 *			Sets up a sigaction structure with SIG_IGN as the handler and
 *			an empty signal mask. Calls sigaction() to apply the settings.
 *			If sigaction fails, returns MSHELL_FAILURE; otherwise returns
 *			MSHELL_SUCCESS.
 *
 * @note	Prevents the shell from quitting when the user types Ctrl+\.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on error.
 */
int			shell_sigquit(void);

/*--------------------------------------------------------------------------*/
/*-----------------------------SHELL TROUBLESHOOT---------------------------*/
/*--------------------------------------------------------------------------*/

int			shell_hderr(char *limit, char *errft, int hdoc_fd[2], t_body *msh);

int			shell_binerr(int binerr, char *cmd, char *usage, char flag);

/**
 * @brief	Cleans up allocated resources of the shell.
 *
 * 			Frees memory and clears structures related to environment
 * 			variables, child processes, command lists, token lists, and
 * 			error file descriptors. Optionally clears readline history if
 * 			the shell is in interactive mode and end flag is set.
 *
 * @param	end		Integer flag indicating if shell is terminating. If
 *					true, additional cleanup is performed for interactive
 *					mode and environment variables.
 * @param	msh		Pointer to the shell state structure containing all
 *					allocated resources and metadata.
 *
 * @note	After cleanup, all pointers in the msh structure are set to NULL.
 * @note	Do not use msh pointers after this function without reinitializing.
 */
void		shell_cleanup(int end, t_body *msh);

/**
 * @brief	Frees a t_var structure and its allocated name and value strings.
 *
 *			Frees the name and value fields if they exist, then frees the
 *			t_var structure itself.
 *
 * @param	content	Pointer to the t_var structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists
 * 			of environment variables.
 * @note	After calling, the pointer to the variable becomes invalid.
 */
void		shell_delenvar(void *content);

/**
 * @brief	Frees a t_cmd structure and all its associated resources.
 *
 *			Frees each string in the argv array and then the array itself.
 *			Frees the pathname string if it exists. Sets pointers to NULL
 *			before freeing the t_cmd structure.
 *
 * @param	content	Pointer to the t_cmd structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists
 * 			of commands.
 * @note	After calling, the pointer to the command becomes invalid.
 */
void		shell_delcmd(void *content);

/**
 * @brief	Frees a t_token structure and its allocated members.
 *
 *			Frees the str and mask fields of the token if they exist,
 *			sets them to NULL, and then frees the t_token structure itself.
 *
 * @param	content	Pointer to the t_token structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists of tokens.
 * @note	After calling, the pointer to the token becomes invalid.
 */
void		shell_deltkn(void *content);

/**
 * @brief	Reports a syntax error for an unexpected token in the shell.
 *
 *			Prints a formatted error message to STDERR indicating the
 *			unexpected token. Includes the line number if the shell is
 *			not in interactive mode. Sets msh->exit_no to MSHELL_MISSUSE.
 *
 * @param	next	Pointer to the unexpected token string, or "newline" if
 * 					NULL.
 * @param	msh		Pointer to the main shell structure (t_body) to update
 * 					exit_no.
 *
 * @note	Returns the error code MSHELL_MISSUSE.
 * @note	Does not terminate the shell; execution continues after reporting.
 *
 * @return	MSHELL_MISSUSE.
 */
int			shell_parserr(const char *next, t_body *msh);

/**
 * @brief	Reports an error related to redirection or pipe operations.
 *
 *			Prints a formatted error message to STDERR using perror(). If
 *			argv is NULL, defaults to "pipe". Returns the provided exit code.
 *
 * @param	exit_no	The exit code to return after reporting the error.
 * @param	argv	Pointer to the string describing the failing operation,
 *			or NULL to default to "pipe".
 *
 * @note	Does not terminate the shell; caller handles continuation or exit.
 *
 * @return	The provided exit code (exit_no).
 */
int			shell_redirerr(int exit_no, char *argv);

/**
 * @brief	Forcefully exits the shell, optionally printing an error.
 *
 *			Calls shell_cleanup() to free all resources. If errno is set,
 *			prints a formatted error message including the argv context.
 *			Restores terminal attributes if necessary before exiting.
 *
 * @param	exit_no The exit code to terminate the shell with.
 * @param	argv 	Context string, usually the command that caused the error.
 * @param	msh		Pointer to the main shell structure (t_body) for cleanup.
 *
 * @note	Exits the program immediately; does not return.
 * @note	Use MSHELL_FATAL for unrecoverable errors.
 */
void		shell_forcend(int exit_no, const char *argv, t_body *msh);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif