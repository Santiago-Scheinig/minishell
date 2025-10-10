/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 08:58:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_H
# define MSH_PSR_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

/*--------------------------------------------------------------------------*/
/*------------------------------SHELL STRUCTURES----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Stores lengths related to environment variable expansion.
 *
 *			Used to track the length of the variable name and the
 *			length of its corresponding value string during parsing
 *			and mask updates.
 */
typedef struct s_envar_pair
{
	int	var;	// < Length of the environment variable name
	int	val;	// < Length of the variable's value string
}	t_envar_pair;

/**
 * @brief	Pairs file descriptor and exit code for heredoc operations.
 *
 *			Used during heredoc processing to pass the write end of
 *			the heredoc pipe and the shell's current exit status to
 *			helper functions.
 */
typedef struct s_hdoc_pair
{
	int fd;		// < File descriptor for heredoc writing
	int	exit_no;// < Current shell exit code
}	t_hdoc_pair;

/**
 * @brief	Heredoc tracking structure for the shell.
 *
 *			Contains the heredoc limiter string, line counter, and
 *			the pipe fds used to capture heredoc content from stdin.
 */
typedef struct s_hdoc_tsh
{
	char	*limitator;	// < String marking the end of heredoc input
	int		line;		// < Line number or count in heredoc reading
	int		heredoc[2];	// < Pipe fds for heredoc input/output
}	t_hdoc_tsh;

/*--------------------------------------------------------------------------*/
/*------------------------------PARSER LIBRARY------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Builds command structures from the token list.
 *
 *			Iterates through all tokens and constructs a sequence of
 *			t_cmd structures representing piped commands. Handles
 *			redirections, argument insertion, and pipe boundaries.
 *
 *			Calls cmdupd_redir() for redirections, cmdupd_argv() for
 *			arguments, and cmdupd_save() when a PIPE token is found.
 *
 *			Static helpers:
 *
 *				- cmd_init():	Initializes new command blocks.
 *				- cmdupd_argv():	Adds arguments to command argv.
 *				- cmdupd_save():	Appends completed command nodes.
 *
 * @param	msh	Main shell structure (contains token list and cmd list).
 *
 * @note	Forces shell termination on any allocation failure.
 */
void		parser_cmds(t_body *msh);

/**
 * @brief	Expands environment variables in all word tokens.
 *
 *			Traverses the token list, expanding variables within each
 *			token string using envar_syntax(). Tokens marked with 'N'
 *			in their mask after expansion are split again via
 *			envar_tokenization() to handle multi-word expansions.
 *
 *			Static helpers:
 *
 *				- envar_syntax():	Performs actual string expansion.
 *				- envar_tokenization():	Splits expanded tokens.
 *
 * @param	msh	Pointer to main shell structure (t_body).
 *
 * @note	Modifies token list in-place. May allocate new tokens.
 */
void		parser_envar(t_body *msh);

/**
 * @brief	Parses shell input into tokens for command execution.
 *
 *			Reads user input, splits it into tokens separated by
 *			spaces or special operators (<<, >>, <, >, |). Handles
 *			interactive history and empty input recursion.
 *
 *			Static helpers:
 *
 *				- input_reader():	Handles prompt or stdin reading.
 *
 * @param	split	Output pointer to array of tokenized strings.
 * @param	msh		Pointer to main shell structure (t_body).
 *
 * @note	Frees the input string after splitting.
 * @note	Recursively re-prompts on empty input.
 */
void		parser_input(char ***split, t_body *msh);

/**
 * @brief	Converts tokenized strings into a linked token list.
 *
 *			Creates new token objects from each split string using
 *			shell_newtkn() and links them in msh->head_token. Then
 *			performs syntax validation via token_syntax().
 *
 *			On allocation failure, frees remaining data and exits.
 *
 *			Static helpers:
 *
 *				- token_syntax():	Performs full token validation.
 *
 * @param	split	Array of token strings (from parser_input()).
 * @param	msh		Pointer to main shell structure (t_body).
 *
 * @note	Frees split array after processing.
 *
 * @return	Result of token_syntax(): 0 on success, non-zero on error.
 */
int			parser_token(char **split, t_body *msh);

/*--------------------------------------------------------------------------*/
/*--------------------------------ENVAR UTILS-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Updates the mask string after environment variable expansion.
 *
 *			Adjusts the mask for a string segment expanded from an
 *			environment variable, ensuring character classification
 *			remains consistent (e.g., quoted, normal, expanded).
 *
 *			Calls exp_mask() to rebuild the mask substring around the
 *			expansion site.
 *
 * @param	start	Index in str where expansion begins.
 * @param	str		Original string containing the variable.
 * @param	value	Value string of the variable (may be NULL).
 * @param	mask	Pointer to mask string associated with str.
 *
 * @note	If mask is NULL, the function does nothing.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int			envar_mask(int start, char *str, char *value, char **mask);

/**
 * @brief	Parses and expands environment variables in a string.
 *
 *			Iterates through 'str', replacing every valid '$VAR' or
 *			'$?' pattern with its corresponding value. Ignores '$'
 *			inside single-quoted segments marked in 'mask'.
 *
 *			Calls envar_init() for named variables and exp_exitno()
 *			for special '$?' cases. The mask is updated accordingly.
 *
 *			Static helpers:
 *
 *				- envar_init():		Expands named variables.
 *				- exp_exitno():		Handles '$?' exit code expansion.
 *
 * @param	exit_no	Last command's exit status (for $? replacement).
 * @param	str		Pointer to string to expand.
 * @param	mask	Optional mask string marking quoted regions.
 * @param	head_envar	Linked list of environment variables.
 *
 * @note	Expands variables only outside single quotes.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int			envar_syntax(int exit_no, char **str, char **mask, t_list *envar);

/*--------------------------------------------------------------------------*/
/*---------------------------------CMD UTILS--------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Resets command data after a redirection error.
 *
 *			Frees argv and pathname from the current command and
 *			clears its structure. Then skips remaining tokens until
 *			the next PIPE, allowing parsing to continue cleanly for
 *			the following command segment.
 *
 * @param	lst_token	Current token in the token list.
 * @param	new_cmd		Double pointer to the active command struct.
 *
 * @note	Used after a redirection error to safely resume parsing.
 *
 * @return	Pointer to the token before the next PIPE or END token.
 */
t_list		*cmdupd_err(t_list *lst_token, t_cmd **new_cmd, t_body *msh);

/**
 * @brief	Handles all command redirection updates.
 *
 *			Determines the redirection type from aux->type and
 *			dispatches to the proper helper for handling input,
 *			output, append, or heredoc redirections.
 *
 *			Static helpers:
 *
 *				- cmdupd_infile():	Handles '<' input redirection.
 *				- cmdupd_outfile():	Handles '>' and '>>' output.
 *				- cmdupd_heredoc():	Handles heredoc linking.
 *
 * @param	aux			Current token (redir operator).
 * @param	aux_next	Next token (target filename or heredoc).
 * @param	new			Active command structure.
 *
 * @note	Returns failure if an unknown redirection type is found.
 *
 * @return	MSHELL_SUCCESS or MSHELL_FAILURE depending on result.
 */
int			cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif