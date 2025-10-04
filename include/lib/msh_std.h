/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_std.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:39:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:11:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_STD_H
# define MSH_STD_H

# include "libft.h"
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
	MSHELL_CMD_INVEXE = 126,	// Command invoked cannot execute
	MSHELL_CMD_NOTEXE,		// Command not found or not executable
	MSHELL_SIG_HANDLR,		// Error in setting up signal handler
}	t_error;

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
 * @note	err_fd is allocated and owned by the shell to handle child
 *			process errors. Freed during cleanup.
 * @note	exit_ft stores the function name that caused the last failure
 *			in forcend().
 * @note	childs_pid is allocated and freed on cleanup, tracking all
 *			child process PIDs.
 * @note	lst_t_cmd, lst_t_var, lst_t_token are linked lists of commands,
 *			environment variables, and tokens, respectively. Each node's
 *			content must be cast to the appropriate type.
 */
typedef struct s_body
{
	struct termios	orig_term;	// < Original terminal settings
	int				interactive;// < If shell runs interactively, otherwise 0
	int				line;		// < Current input line number or count
	int				*err_fd;	// < Pipe fds for child error handling
	int				exit_no;	// < Last command's exit code
	const char		*exit_ft;	// < Function name causing last failure
	char			**envp;		// < Array of environment strings
	char			*input;		// < Raw input line from the user
	pid_t			*childs_pid;// < Array of child PIDs
	t_list			*lst_t_cmd;	// < Linked list of t_cmd commands
	t_list			*lst_t_var;	// < Linked list of t_var environment variables
	t_list			*lst_t_token;//< Linked list of t_token parsed tokens
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
 * @brief Adds a new token node or rewrites the current one in a list.
 *
 * Creates a new t_token node with default type WORD and inserts it
 * after the current node in lst_t_token. If rewrite == 0, replaces
 * the content of the current node instead of creating a new node.
 *
 * @param str 		Pointer to a string to store in the new or replaced token.
 * @param rewrite 	If 0, replaces the current token content; otherwise
 *					adds a new node after the current one.
 * @param lst_t_token Pointer to the current node in the token list.
 *
 * @note	Memory for the new t_token and its mask is allocated using
 *			token_dup. Caller is responsible for freeing the list.
 * @note	replace_token() never fails.
 *
 * @return	0 on success, 1 on memory allocation failure.
 */
int		shell_addlst_token(char *str, int rewrite, t_list *lst_t_token);

/**
 * @brief Duplicates the environment variable array with SHLVL adjustment.
 *
 * Allocates a new array of strings, duplicating each entry from envp.
 * If an entry starts with "SHLVL=", it is processed by check_value()
 * to increment or reset the shell level. Other entries are duplicated
 * normally.
 *
 * @param envp	Pointer to the original environment array.
 *
 * @note	Memory is allocated for the array and each string; the caller
 *			is responsible for freeing all of it.
 * @note	Returns NULL if memory allocation fails at any point.
 *
 * @return	Pointer to the newly allocated environment array, or NULL
 *			on allocation failure.
 */
char	**shell_envpdup(const char **envp);

/**
 * @brief Retrieves the value of an environment variable from a list.
 *
 * Searches lst_var for a variable with the given name. Sorts the list
 * first using shell_sortenv(). Returns the value string if found and
 * non-empty, otherwise returns NULL.
 *
 * @param lst_var Pointer to the linked list of t_var environment variables.
 * @param name Name of the environment variable to search for.
 *
 * @note	Returns NULL if the variable is not found or if its value is empty.
 * @note	The returned pointer points to the string stored in the list;
 *			do not free it directly.
 *
 * @return	Pointer to the variable's value string, or NULL if not found 
 * 			or empty.
 */
char	*shell_getenv(t_list *lst_t_var, const char *name);

/**
 * @brief Clears and frees a linked list.
 *
 * Iterates through the list pointed to by lst, applying the provided
 * del function to each node and its content. Frees each node and
 * sets the list pointer to NULL at the end.
 *
 * @param lst	Pointer to the pointer of the linked list to clear.
 * @param del	Function pointer to a function that deletes a node's content.
 *
 * @note	The del function must handle freeing any allocated memory
 *			in the node content. The list structure itself is freed
 *			by this function.
 */
void	shell_lstclear(t_list **lst, void (*del)(t_list *, void (*)(void *)));

/**
 * @brief Deletes a token node and its associated memory.
 *
 * Frees the memory used by a t_token stored in lst->content, including
 * the string and mask. Also frees the node itself.
 *
 * @param lst	Pointer to the token list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes lst->content points to a valid t_token.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_tkn(t_list *lst, void (*del)(void *));

/**
 * @brief Deletes a command node and its associated memory.
 *
 * Frees the memory used by a t_cmd stored in lst->content, including
 * the pathname string and the argv array with all its elements.
 * Also frees the node itself.
 *
 * @param lst	Pointer to the command list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes lst->content points to a valid t_cmd.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_cmd(t_list *lst, void (*del)(void *));

/**
 * @brief Deletes an environment variable node and its associated memory.
 *
 * Frees the memory used by a t_var stored in list->content, including
 * the name and value strings. Also frees the node itself.
 *
 * @param list	Pointer to the environment variable list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes list->content points to a valid t_var.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_var(t_list *lst, void (*del)(void *));

/**
 * @brief Copies memory from src to dest using a mask.
 *
 * Moves n bytes from src to dest. If dest > src, copies bytes
 * downward to avoid overlap; otherwise, copies upward. The mask
 * array controls special handling of specific bytes 
 * (implementation-dependent).
 *
 * @param dest	Pointer to the destination memory.
 * @param src	Pointer to the source memory.
 * @param mask	Pointer to a mask array affecting how bytes are moved.
 * @param n		Number of bytes to copy.
 *
 * @note	Returns dest even if dest and src are NULL.
 * @note	Uses iq_movedn and iq_moveup internally for handling overlap.
 *
 * @return	Pointer to the destination memory.
 */
void	*shell_memmove(void *dest, void *src, void *mask, size_t n);

/**
 * @brief Creates a linked list of environment variable nodes from an array.
 *
 * Iterates over the envp array, creating a t_var structure for each
 * environment string using create_envp(), then stores each in a new
 * linked list node. Returns the head of the list.
 *
 * @param envp	Pointer to the array of environment strings.
 *
 * @note	Memory is allocated for each t_var and list node. If allocation
 *			fails, previously allocated nodes are freed using shell_lstclear().
 *
 * @return	Pointer to the head of the newly created linked list, or NULL
 *			on failure.
 */
t_list	*shell_newlst_var(char **envp);

/**
 * @brief Creates a t_var structure from an environment string.
 *
 * Allocates and initializes a t_var structure from a string of the form
 * "NAME=value". Sets the name and value fields and marks the variable
 * as exported or not based on the export parameter.
 *
 * @param envp		Pointer to the environment string to convert.
 * @param export	Integer flag indicating if the variable is exported (1)
 *               	or not (0).
 *
 * @note	Memory is allocated for the t_var structure and its contents.
 *			Returns NULL if allocation fails.
 *
 * @return	Pointer to the newly created t_var structure, or NULL on failure.
 */
t_var	*shell_create_var(const char *envp, int export);

/**
 * @brief Expands the PS1 shell prompt using environment variables.
 *
 * Retrieves the PS1 variable from the environment list. If not set,
 * uses an empty string. Resolves the current working directory using
 * path_cwd() and generates the final prompt string via pwd_name().
 *
 * @param lst_t_var 	Pointer to the environment variable linked list 
 * 						(t_list of t_var).
 *
 * @note	Memory is allocated for the resulting prompt string and must
 *			be freed by the caller.
 * @note	If PS1 is not set, an empty string is used as default.
 *
 * @return	Pointer to the dynamically allocated expanded prompt string,
 *			or NULL on failure.
 */
char	*shell_pmtexp(t_list *lst_t_var);

/**
 * @brief Creates an array of strings representing key shell prompt variables.
 *
 * Allocates an array of 5 strings and populates it with:
 *   0: PS1 prompt string (from ps1_init)
 *   1: Current working directory (from pwd_init)
 *   2: Last executed command (from lastcmd_init)
 *   3: SHLVL=1 string
 *   4: NULL terminator
 *
 * @param lst_t_var	Pointer to the environment variable linked list
 * 					(t_list of t_var).
 *
 * @note	Memory is allocated for both the array and each string. Caller
 *			is responsible for freeing them.
 * @note	Returns NULL if any allocation fails.
 *
 * @return Pointer to the dynamically allocated array of prompt-related strings,
 *			or NULL on failure.
 */
char	**shell_pmtstr(t_list *lst_t_var);

/**
 * @brief Sorts a linked list of environment variables alphabetically by name.
 *
 * Performs a simple bubble sort on the linked list of t_var structures
 * pointed to by head. Uses swap_env() to reorder adjacent nodes when needed.
 *
 * @param var 	Pointer to the head of the linked list of 
 * 				environment variables.
 *
 * @note  Sorting is done in-place; no new nodes are allocated.
 * @note  Each node's content must be cast to t_var to access name strings.
 */
void	shell_sortenv(t_list **var);

/**
 * @brief Splits a shell input string into tokens.
 *
 * Counts the number of operators and words in the input string, allocates
 * an array of strings, and fills it with each parsed token.
 *
 * @param str  The input shell command string to split.
 *
 * @return  A NULL-terminated array of strings (tokens), or NULL on failure.
 *          If the input string is empty, returns an array with a single NULL.
 */
char	**shell_split(const char *str);


/*--------------------------------------------------------------------------*/
/*-------------------------------SHELL HANDLERS-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief Restores default handlers for SIGINT, SIGQUIT, and SIGPIPE.
 *
 * Sets the specified signals to their default behavior (SIG_DFL), which
 * is typically used before executing child processes to ensure that
 * signals are handled normally. This allows child processes to respond
 * to interrupts or quits as expected.
 *
 * @note	Does not modify g_signal_received; only affects signal handling.
 * @note	Useful for ensuring that child processes are not affected by
 *			custom shell signal handlers.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigdfl(void);

/**
 * @brief Configures the shell to ignore SIGINT and SIGQUIT signals.
 *
 * Sets the shell to ignore SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) using
 * sigaction. The signal mask blocks SIGINT and SIGQUIT during handling,
 * and SA_RESTART is used to automatically restart interrupted system calls.
 *
 * @note	Specifically used before heredoc input to allow the user to
 *			enter data without generating new prompts or interruptions.
 * @note	Does not modify g_signal_received.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigign(void);

/**
 * @brief Sets up a SIGINT handler for non-interactive shell mode.
 *
 * Configures the shell to handle SIGINT (usually Ctrl+C) by calling
 * set_flag, which sets g_signal_received to 1. Other parts of the shell
 * check this flag to detect abrupt termination and perform cleanup.
 *
 * @note	Blocks SIGQUIT and SIGINT during handling, and uses SA_RESTART
 *			to automatically restart interrupted system calls.
 * @note	Used only in non-interactive mode to ensure proper cleanup.
 * @note	set_flag does not perform cleanup itself; it only sets the flag.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int shell_signint(void);

/**
 * @brief Sets up a SIGINT handler for the interactive shell prompt.
 *
 * Configures the shell to handle SIGINT (usually Ctrl+C) by calling
 * new_prompt. This refreshes the prompt, prints a newline, clears the
 * current readline buffer, and forces a fresh prompt display.
 *
 * @note	Blocks SIGQUIT and SIGINT during handling, and uses SA_RESTART
 *			to automatically restart interrupted system calls.
 * @note	new_prompt sets g_signal_received to indicate an interrupt
 *			occurred, allowing other parts of the shell to respond if
 *			needed.
 * @note	Used only in interactive mode.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigint(void);

/**
 * @brief Configures the shell to ignore the SIGQUIT signal.
 *
 * Sets SIGQUIT (usually triggered by Ctrl+\) to be ignored by the shell
 * process using sigaction. This ensures that the shell is not terminated
 * or dumped by SIGQUIT while running interactively or in non-interactive
 * mode.
 *
 * @note	Sets sa_handler = SIG_IGN and clears the signal mask.
 * @note	Does not affect child processes; they may still receive SIGQUIT
 *			unless modified separately.
 * @note	Does not modify g_signal_received because SIGQUIT is ignored.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigquit(void);

/*--------------------------------------------------------------------------*/
/*-----------------------------SHELL TROUBLESHOOT---------------------------*/
/*--------------------------------------------------------------------------*/

void	cleanup(t_body *minishell);

/**
 * COMENT PENDING
 */
int		forcend(t_body *msh, const char *argv, int exit_no);

int		redirend(char *argv, t_error number);

int		parsend(const char *next, t_error number, t_body *minishell);

void	end_minishell(t_body *minishell);

#endif