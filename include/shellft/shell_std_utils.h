/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_std_utils.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:24:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:22:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_STD_UTILS_H
# define SHELL_STD_UTILS_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"

/*--------------------------------------------------------------------------*/
/*------------------------------------ENV-----------------------------------*/
/*--------------------------------------------------------------------------*/
/**
 * @brief	Extracts the name part of an environment variable string.
 *
 *			Scans the input string until the first '=' character or the end
 *			of the string, and duplicates that portion as the variable name.
 *
 * @param	var	String containing the variable in the form "NAME=VALUE"
 *				or simply "NAME".
 *
 * @note	The returned string is dynamically allocated and must be freed
 *			by the caller. If memory allocation fails, returns NULL.
 *
 * @return	Pointer to the duplicated variable name, or NULL on failure.
 */
char		*get_name(char *var);

/**
 * @brief	Extracts the value part of an environment variable string.
 *
 *			Searches for the '=' character in the given variable string.
 *			If found, duplicates the portion after it as the variable value.
 *			If '=' is missing, assigns NULL to the output pointer.
 *
 * @param	var		String containing the variable in the form "NAME=VALUE".
 * @param	value	Double pointer where the duplicated value string is stored.
 *
 * @note	If the variable has no value (e.g., "NAME="), an empty string is
 *			duplicated instead of NULL. Caller is responsible for freeing it.
 *
 * @return	MSHELL_SUCCESS on success, or MSHELL_FAILURE if memory allocation
 *			for the duplicated value fails.
 */
int			get_value(char *var, char **value);

/**
 * @brief	Creates a string representing the current working directory.
 *
 * 			Calls getcwd() to retrieve the current working directory and
 * 			prefixes it with "PWD=" to form a standard environment variable
 * 			string.
 *
 * @note	The returned string must be freed by the caller to avoid memory leaks.
 *
 * @return	Pointer to a newly allocated string in the format "PWD=/path/to/dir",
 * 			or NULL if getcwd() fails or memory allocation fails.
 */
char		*pwdstr(void);

/*--------------------------------------------------------------------------*/
/*------------------------------------PMT-----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Calculates the length of the formatted shell prompt string.
 *
 *			Iterates through the PS1 format string. For recognized escape
 *			sequences, adds the length of the corresponding value (user or
 *			path). For other characters, increments length by 1. Handles
 *			backslash escapes properly.
 *
 * @param	ps1		Pointer to the PS1 format string.
 * @param	user	Pointer to the username string.
 * @param	path	Pointer to the path string.
 *
 * @note	Returns 0 if ps1 is NULL.
 * @note	Only handles escape sequences '\u' for user and '\w' for path.
 *
 * @return	Total length required for the formatted prompt string.
 */
size_t		prompt_len(char *ps1, char *user, char *path);

/**
 * Need fix
 */
char		*transform_format(char *tmp, char *ps1, char *user, char *path);

/*--------------------------------------------------------------------------*/
/*------------------------------------SIG-----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Signal handler to refresh the shell prompt.
 *
 *			Sets the shell signal flag using sigdlr_setflg(), prints a newline,
 *			clears the current readline input, moves to a new line, and
 *			redisplays the prompt using the readline library functions.
 *
 * @param	signum	The signal number received (ignored in this function).
 *
 * @note	Designed to handle SIGINT (Ctrl+C) during prompt input.
 * @note	Does not terminate the shell; only resets the input line.
 */
void		sigdlr_newpmt(int signum);

/**
 * @brief	Sets the global signal variable to the received signal number.
 *
 *			Updates the g_signal_received variable with the value of signum,
 *			allowing the shell to know exactly which signal was caught.
 *
 * @param	signum	The signal number received.
 *
 * @note	Used in conjunction with signal handlers to track specific signals.
 */
void		sigdlr_setflg(int signum);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Identifies the type of a shell token from a string.
 *
 *			Checks the input string against known shell operators:
 *			PIPE ("|"), REDIR_APPEND (">>"), HEREDOC ("<<"), REDIR_IN ("<"),
 *			and REDIR_OUT (">"). Returns WORD for non-operator strings and
 *			END for NULL or empty strings.
 *
 * @param	str	Pointer to the string to analyze.
 *
 * @note	Token types must match the predefined constants used in the shell.
 *
 * @return	Integer representing the token type (END, PIPE, REDIR_APPEND,
 *			HEREDOC, REDIR_IN, REDIR_OUT, or WORD).
 */
int			get_token_type(char *str);

/**
 * @brief	Determines if the string starts with a word delimiter or operator.
 *
 *			Checks the input string against known shell operators: PIPE ("|"),
 *			REDIR_APPEND (">>"), HEREDOC ("<<"), REDIR_IN ("<"), REDIR_OUT
 *			(">"), and space (" ") as a word delimiter. Returns WORD if none
 *			match.
 *
 * @param	str	Pointer to the string to analyze.
 *
 * @note	The returned values correspond to predefined shell token constants.
 *
 * @return	Integer representing the type of delimiter or operator, or WORD.
 */
int			is_divisor(char *str);

/**
 * @brief	Frees a partially allocated array of strings and returns NULL.
 *
 *			Iterates through the array of strings up to the specified index,
 *			freeing each allocated string. Finally, frees the array itself
 *			and returns NULL to simplify error handling.
 *
 * @param	wrdstr	Pointer to the array of strings to free.
 * @param	index	Number of elements to free from the array.
 *
 * @note	Use this function when allocation of string arrays fails mid-way.
 *
 * @return	NULL.
 */
void		*memfree(char **wrdstr, int index);

/**
 * @brief	Returns the length in characters of a given shell operator type.
 *
 *			Checks the operator type and returns 1 for single-character
 *			operators (PIPE, REDIR_IN, REDIR_OUT) and 2 for double-character
 *			operators (HEREDOC, REDIR_APPEND). Returns 0 for unknown types.
 *
 * @param	type	Integer representing the token/operator type.
 *
 * @note	Operator types must match the predefined constants used by the shell.
 *
 * @return	Length of the operator in characters, or 0 if type is unknown.
 */
int			operator_len(int type);

/**
 * @brief	Finds the next operator in a string, ignoring quoted sections.
 *
 *			Iterates through the string, skipping characters inside single
 *			or double quotes. Uses get_token_type() to detect operator
 *			characters. Returns a pointer to the first operator found, or
 *			NULL if none exist.
 *
 * @param	str	Pointer to the input string to search for operators.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 * @note	Returns NULL if no operator is found in the string.
 *
 * @return	Pointer to the first operator character in str, or NULL if none.
 */
const char	*operator_strchr(const char *str);

/**
 * @brief	Calculates the length of the next word, ignoring quoted parts.
 *
 *			Iterates through the string until a divisor character is reached,
 *			skipping characters inside single or double quotes. Properly handles
 *			quoted sections to include them in the word length.
 *
 * @param	str	Pointer to the input string to measure.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 *
 * @return	Length of the next word in characters.
 */
int			word_len(const char *str);

/**
 * @brief	Finds the next word delimiter in a string, ignoring quoted parts.
 *
 *			Iterates through the string until a divisor character is found,
 *			using is_divisor() to detect delimiters. Skips over characters
 *			enclosed in single or double quotes. Returns a pointer to the
 *			first delimiter found, or NULL if none exist.
 *
 * @param	str	Pointer to the input string to search for word delimiters.
 *
 * @note	Quotes must be properly closed; otherwise, behavior may be undefined.
 * @note	Returns NULL if no delimiter is found in the string.
 *
 * @return	Pointer to the first delimiter character in str, or NULL if none.
 */
const char	*word_strchr(const char *str);

#endif