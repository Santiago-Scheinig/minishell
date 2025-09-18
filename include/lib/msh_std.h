/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_std.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:39:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 19:59:30 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_STD_H
# define MSH_STD_H

# include "libft.h"
# include <sys/types.h>
# include <termios.h>

/*Id like to change this to a envp structure, inside a t_list structure.*/
/*If the only functions that are able to edit this, are shell functions,*/
/*Then we put this inside of shellft.h instead, more clean.				*/
typedef struct s_var
{
	char				*value;
	char				*name;
	int					exported;
}	t_var;

typedef struct s_files
{
	int	oldin;
	int	oldout;
	int	exein;
	int	exeout;
}	t_files;

typedef struct s_cmd
{
	int		built_in;
	int		heredoc[2];
	char	*limitator;
	char	*pathname;
	char	**argv;
	t_files		fd;
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
	t_token_type	type;	//	The type of token.
}	t_token;

/**
 * Struct used to save the enviroment variables of the minishell.
 * 
 * @param term			Termios structure linked to the terminal initialization
 * @param commands
 * @param input
 * @param childs_pid
 * @param errno
 */
typedef struct s_body
{
	struct termios	orig_term;
	int				exit_status;
	int				interactive;
	int				line;
	char			**envp;//A copy of the original envp + post modifications
	char			*home;
	char			*input;//needed for history?
	pid_t			*childs_pid;
	t_list			*cmd_lst;
	t_list			*envp_lst;
	t_list			*token_lst;
}	t_body;

int	is_divisor(char *str);

int	get_token_type(char *str);

/**
 * Frees every node included on the LIST HEAD. 
 * @param lst The LIST HEAD to free.
 * @param del The function used to free each node.
 * @note In general, the del function should be free().
 */
void	shell_lstclear(t_list **lst, void (*del)(t_list *, void (*)(void *)));

/**
 * Frees every pointer on an ARRAY of STRINGS and the ARRAY pointer, even
 * if it's not NULL terminated.
 * @param wrdstr The ARRAY of STRINGS to free.
 * @param index The amount of STRINGS to free inside of the array.
 */
void	*memfree(char **wrdstr, int index);

/**
 * Returns the proper length of the operator type sent as argument.
 * @param type The T_TOKEN_TYPE that describers the operator type.
 * @return The lenght of the operator type.
 * @note If the operator isn't valid, returns 0.
 */
int	operator_len(int type);

/**
 * Counts the lenght of the first word on a STRING, until it reaches a
 * space, shell operator or '\0'
 * @param s The STRING where to count the lenght of the first word.
 * @return An INT with the lenght of the word.
 * @note This function will ignore any divisor coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
int	word_len(const char *s);

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
char	*shell_substr(char *s, char *mask, unsigned int start, size_t len);

/**
 * Searches for the first ocurrance of a space or an operator on a STRING.
 * @param s The STRING where to find the ocurrance.
 * @return A pointer to a position of the STRING with the first 
 * character ocurrance. If no ocurrance is found, returns NULL. 
 * @note This function will ignore any character coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
const char	*shell_word_strchr(const char *s);

char	*shell_getenv(t_list *lst_var, const char *name);

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
char	**shell_split(const char *s);

/**
 * Frees the content of a TOKEN NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void	shell_lstdeltkn(t_list *lst, void (*del)(void *));

/**
 * Frees the content of a COMAND NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void	shell_lstdelcmd(t_list *lst, void (*del)(void *));

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
int		shell_addlst_token(t_list *token_list, char *str, int start);

/**
 * Creates and allocates a new STRING resultant of the trimming of a 
 * CONST STRING set in a CONST STRING s1.
 * @param s1 The CONST STRING to be compared for trimming.
 * @param set The character base to be trimmed.
 * @return A pointer to the new STRING resultant of the trimming.
 * @note - Cuts every match of 'set' from the start of 's1' until a non match.
 * @note - Cuts every match of 'set' from the end of 's1' until a non match.
 */
char	*shell_strtrim(char const *s1, char const *mask, char const *set);

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
void	*shell_memmove(void *dest, void *src, void *mask, size_t n);

void	shell_lstdelvar(t_list *list, void (*del)(void *));

char	**shell_envpdup(const char **envp);

t_list	*shell_newlst_var(char **envp);

void	shell_sortenv(t_list **head);

size_t	prompt_len(char *ps1, char *user, char *path);

char	*transform_format(char *tmp, char *ps1, char *user, char *path);

char	**shell_pmtstr(t_list *envp_lst);

t_var	*create_envp(const char *envp, int export);

char	*shell_pmtexp(t_list *envp);

#endif