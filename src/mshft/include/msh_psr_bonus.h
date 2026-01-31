/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:06:09 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:26:26 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_BONUS_H
# define MSH_PSR_BONUS_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"
# include <dirent.h>

/*--------------------------------------------------------------------------*/
/*---------------------------------STRUCTURES-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Defines types of logical tokens in shell command parsing.
 *
 *			Used to classify each token in logical command expressions,
 *			such as parentheses and logical operators, for syntax
 *			checking and execution order evaluation.
 */
typedef enum e_logic_type
{
	LPARENTHESIS,	// < Left parenthesis '(' token
	RPARENTHESIS,	// < Right parenthesis ')' token
	AND,			// < Logical AND '&&' token
	OR,				// < Logical OR '||' token
	INPUT,			// < Regular command or input token
	UNSET,			// < Uninitialized or unknown token type
}	t_logic_type;

/**
 * @brief	Represents a single logical token in a command sequence.
 *
 *			Stores the token string and its corresponding type
 *			as defined by t_logic_type, for parsing and execution.
 */
typedef struct s_logic
{
	char	*str;	// < String content of the logical token
	int		type;	// < Type of the token (t_logic_type)
}	t_logic;

/*--------------------------------------------------------------------------*/
/*------------------------------PARSER LIBRARY------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Copies a directory entry name into a dynamic names array.
 *
 * 			Reallocates the array to add one more entry, then duplicates
 *			the directory name into the new slot.
 *
 * @param	names	Double pointer to the array of names.
 * @param	dir		Name of the directory entry to copy.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
int			copy_name(char ***names, char *dir);

/**
 * @brief	Main logic parser for input with &&, ||, and parentheses.
 *
 *			Splits input based on operators and spaces, then performs
 *			syntax validation.
 *
 * @param	input	Input string from user.
 * @param	msh		Main shell structure.
 *
 * @return	MSHELL_SUCCESS if input is syntactically correct,
 *			MSHELL_FAILURE otherwise.
 */
int			logic_parser(char *input, t_body *msh);

/**
 * @brief	Expands wildcard patterns in tokens to matching filenames.
 *
 *			Builds a list of names in the current directory, then for
 *			each token in msh->head_token, finds matches via wildcard().
 *			Matching strings are added as new tokens. Frees temporary
 *			memory arrays. On allocation failure, calls shell_forcend().
 *
 * @param	msh		Shell state containing token list and environment.
 *
 * @return	MSHELL_SUCCESS on success.
 */
int			parser_wildcard(t_body *msh);

/**
 * @brief	Expands wildcard patterns by matching names in a directory.
 *
 * 			Iterates over the names array, matches each name against
 *			the token's wildcard pattern, and appends matches to matches.
 *
 * @param	names	Array of strings (e.g., filenames) to match against.
 * @param	matches	Double pointer to store matched strings.
 * @param	head	Linked list node containing the current token.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
int			wildcard(char **names, char ***matches, t_list *head);

/*--------------------------------------------------------------------------*/
/*--------------------------------SHELL BONUS-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Creates a new t_logic structure from a string token.
 *
 *			Allocates a t_logic struct, sets the string pointer, and
 *			determines the token type (LPARENTHESIS, RPARENTHESIS,
 *			AND, OR, or INPUT) based on the string.
 *
 * @param	str	String token to wrap in t_logic.
 *
 * @return	Pointer to newly allocated t_logic, or NULL on malloc failure.
 */
t_logic		*shell_newlogic(char *str);

/**
 * @brief	Frees a t_logic structure and its contents.
 *
 *			Frees the dynamically allocated string inside t_logic,
 *			then frees the t_logic struct itself.
 *
 * @param	content	Pointer to a t_logic struct to free.
 */
void		shell_dellogic(void *content);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif