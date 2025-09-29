/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_std.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:39:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/27 15:05:11 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_STD_H
# define MSH_STD_H

# include "libft.h"
# include <sys/types.h>
# include <termios.h>

/*--------------------------------------------------------------------------*/
/*------------------------------SHELL STRUCTURES----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Struct used to save the enviroment variables of the minishell.
 */
typedef struct s_body
{
	struct termios	orig_term;
	int				interactive;
	int				line;
	int				err_fd;
	int				exit_no;
	const char		*exit_ft;
	char			**envp;
	char			*input;
	pid_t			*childs_pid;
	t_list			*cmd_lst;
	t_list			*envp_lst;
	t_list			*token_lst;
}	t_body;

/**
 * COMMENT PENDING
 */
typedef struct s_cmd
{
	char	*pathname;
	char	**argv;
	int		infd;
	int		outfd;
	int		pipefd;
}	t_cmd;

/**
 * An enumeration list of token types.
 */
typedef enum e_token_type
{
	WORD,			//	Command or argument
	PIPE,			//	|
	REDIR_IN,		//	<
	REDIR_OUT,		//	>
	REDIR_APPEND,	//	>>
	HEREDOC,		//	<<
	END,			//	End of line
}	t_token_type;

/**
 * A token structure where to save the value and it's type.
 */
typedef struct s_token
{
	char			*str;	//	The raw token string.
	char			*mask;	//	The mask of each character inside the string.
	int				heredoc;
	t_token_type	type;	//	The type of token.
}	t_token;

/**
 * COMMENT PENDING ISMA
 */
typedef struct s_var
{
	char				*value;
	char				*name;
	int					exported;
}	t_var;

/*--------------------------------------------------------------------------*/
/*--------------------------------SHELL LIBRARY-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Frees every node included on the LIST HEAD.
 * 
 * @param lst The LIST HEAD to free.
 * @param del The function used to free each node.
 * @note In general, the del function should be free().
 */
void		shell_lstclear(t_list **lst, void (*del)(t_list *, void (*)(void *)));

/**
 * COMMENT PENDING ISMA
 */
void		shell_sortenv(t_list **head);

/**
 * Creates and allocates a new STRING with len bytes copied from the 
 * CONST STRING starting on the start position.
 * @param s The CONST STRING to copy from.
 * @param start The index of the CONST STRING to start copying from.
 * @param len The amount of bytes to be copied.
 * @return A pointer to a new STRING resultant of the copy.
 * @note - If start > s_len, it returns a pointer to a new STRING 
 * with one '\0' byte allocated.
 * @note - If a single or double quote is found unclosed, it becomes
 * ignored and won't be copied into the new string.
 */
char		*shell_substr(char *s, char *mask, unsigned int start, size_t len);

/**
 * Searches for the first ocurrance of a space or an operator on a STRING.
 * @param s The STRING where to find the ocurrance.
 * @return A pointer to a position of the STRING with the first 
 * character ocurrance. If no ocurrance is found, returns NULL. 
 * @note This function will ignore any character coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
const char	*shell_word_strchr(const char *s);

/**
 * Searches for the first ocurrance of an operator on a STRING.
 * @param s The STRING where to find the ocurrance.
 * @return A pointer to a position of the STRING with the first 
 * character ocurrance. If no ocurrance is found, returns NULL. 
 * @note This function will ignore any character coincidences that happen
 * to be inside of single and double quotes (as long they open and close).
 */
const char	*shell_operator_strchr(const char *s);

/**
 * Splits a STRING into an ARRAY of STRINGS, divided between words and 
 * shell operators, while also using spaces as divisor characters.
 * @param s The STRING to split.
 * @return An ARRAY of STRINGS that includes every divided shell token and is 
 * NULL terminated.
 * @note This function will take any amount of characters that happen to be 
 * inside of single and double quotes (as long they open and close) as 
 * WORD tokens. Additionally, if quotes aren't properly closed, they 
 * are treated as unexistent.
 */
char		**shell_split(const char *s);

/**
 * Frees the content of a TOKEN NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void		shell_lstdeltkn(t_list *lst, void (*del)(void *));

/**
 * Frees the content of a COMAND NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void		shell_lstdelcmd(t_list *lst, void (*del)(void *));

/**
 * Adds a new list node, after the current one, which also includes a 
 * new T_TOKEN content default setted as WORD.
 * 
 * @param token_list A pointer to the current position on the token list.
 * @param str A pointer to the new STRING to use content on the new token.
 * @param start A flag that if equals zero, will replace the content of 
 * the current token node, instead of creating a new node.
 * @return Zero on success. or Malloc error. NEED TO CHANGE THIS LINE!!!!
 */
int			shell_addlst_token(t_list *token_list, char *str, int start);

/**
 * Creates and allocates a new STRING resultant of the trimming of a 
 * CONST STRING set in a CONST STRING s1.
 * @param s1 The CONST STRING to be compared for trimming.
 * @param set The character base to be trimmed.
 * @return A pointer to the new STRING resultant of the trimming.
 * @note - Cuts every match of 'set' from the start of 's1' until a non match.
 * @note - Cuts every match of 'set' from the end of 's1' until a non match.
 */
char		*shell_strtrim(char const *s1, char const *mask, char const *set);

/**
 * Moves n bytes from a src VOID pointer into a dest VOID pointer.
 * @param dest The VOID pointer where to move bytes into.
 * @param src The VOID pointer where to move bytes from.
 * @param n The amount of bytes to be moved.
 * @return The VOID pointer to dest.
 * @note This function modifies the dest VOID pointer and, additionally,
 * checks if the memory position of src is close to dest to avoid loosing
 * information during the movement.
 */
void		*shell_memmove(void *dest, void *src, void *mask, size_t n);

/**
 * Frees the t_var content stored in a list node and then frees the node.
 * 
 * @param list Pointer to the list node containing a t_var structure as
 *		content.
 * @param del  Function used to free allocated memory for each member and
 * 		structures.
 * 
 * This function extracts the t_var structure from the provided list node,
 * frees the name and value strings if present, frees the t_var structure
 * itself, and finally frees the list node using the provided del function.
 * 
 * @note - The del function is expected to behave like free(void *).
 * @note - After calling this function the provided node and its content must
 * 		not be accessed.
 */
void	shell_lstdelvar(t_list *list, void (*del)(void *))

/**
 * Duplicates the process environment array and adjusts SHLVL if present.
 * 
 * @param envp Null-terminated array of environment strings from the parent
 * 		process.
 * 
 * Creates and returns a newly allocated copy of envp. If a "SHLVL=" entry is
 * found the function attempts to increment its numeric value; if the existing
 * value is invalid it sets SHLVL to 1 in the duplicated array.
 * 
 * @return Newly allocated null-terminated array of strings, or NULL on
 * 		allocation failure.
 * @note - Caller is responsible for freeing the returned array
 * 		(each string and the array).
 * @note - On allocation failure any partial allocations are freed before
 * 		returning NULL.
 */
char	**shell_envpdup(const char **envp)

/**
 * Builds a linked list of t_var nodes from a null-terminated envp array.
 *
 * @param envp Null-terminated array of environment strings.
 *
 * Iterates envp, creates a t_var for each entry and appends it to a new list.
 * On allocation failure the function frees any previously allocated nodes and
 * returns NULL.
 *
 * @return Pointer to the head of the newly created list on success, or NULL on
 * 		failure.
 * @note Caller owns the returned list and must free it with shell_lstdelvar.
 */
t_list	*shell_newlst_var(char **envp)

/**
 * COMMENT PENDING ISMA
 */
char		**shell_pmtstr(t_list *envp);

/**
 * COMMENT PENDING ISMA
 */
char		*shell_pmtexp(t_list *envp);

/**
 * Retrieves the value of an environment variable from the variable list.
 * 
 * @param lst_var Pointer to the linked list of environment variables
 * 		(t_var nodes).
 * @param name Name of the environment variable to find
 * 		(null-terminated string).
 * 
 * Searches the sorted variable list for a node whose name matches [name].
 * If found and the variable has a non-empty value, returns a pointer to that value.
 * The returned pointer refers to the string owned by the list node (do not free).
 * 
 * @return Pointer to the value string, or NULL if the variable is not found
 *         or its value is empty.
 * @note - The function sorts a temporary copy of the list pointer before
 * 			searching.
 * @note - Caller must not modify or free the returned pointer.
 */
char	*shell_getenv(t_list *lst_var, const char *name)

/*--------------------------------------------------------------------------*/
/*--------------------------------SHELL UTILS-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Creates a new t_var node from an environment string.
 *
 * @param envp Environment string in the form "NAME" or "NAME=VALUE".
 * @param export Integer flag indicating whether the variable is exported (1)
 * 		or not (0).
 *
 * Allocates a t_var structure and fills its name and value by parsing envp.
 * Sets the exported flag according to the provided parameter.
 *
 * @return Pointer to the newly created t_var on success, or NULL on allocation
 * 		failure.
 * @note Returned structure must be freed by the caller
 * 		(use shell_lstdelvar or equivalent).
 */
t_var	*create_envp(const char *envp, int export)

/**
 * Frees every pointer on an ARRAY of STRINGS and the ARRAY pointer, even
 * if it's not NULL terminated.
 * 
 * @param wrdstr The ARRAY of STRINGS to free.
 * @param index The amount of STRINGS to free inside of the array.
 */
void		*memfree(char **wrdstr, int index);

/**
 * Returns the proper length of the operator type sent as argument.
 * @param type The T_TOKEN_TYPE that describers the operator type.
 * @return The lenght of the operator type.
 * @note If the operator isn't valid, returns 0.
 */
int			operator_len(int type);

/**
 * Counts the lenght of the first word on a STRING, until it reaches a
 * space, shell operator or '\0'
 * @param s The STRING where to count the lenght of the first word.
 * @return An INT with the lenght of the word.
 * @note This function will ignore any divisor coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
int			word_len(const char *s);

/**
 * Verifies and returns if the string is a divisor operator.
 * 
 * @param str A string in which to verify the token type.
 * @note Returns 0 if it's not a divisor operator and  >0 if it is.
 */
int			is_divisor(char *str);

/**
 * Returns a correspondent number to the token type following the 
 * [enum t_token_type] definition.
 * 
 * @param str A string in which to verify the token type.
 */
int			get_token_type(char *str);

/**
 * COMMENT PENDING ISMA
 */
size_t		prompt_len(char *ps1, char *user, char *path);

/**
 * COMMENT PENDING ISMA
 */
char		*transform_format(char *tmp, char *ps1, char *user, char *path);

#endif