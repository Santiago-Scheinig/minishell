/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_std_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:24:41 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:17:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_UTILS_H
# define MSH_UTILS_H

# include "lib/msh_std.h"

/**
 * @brief Checks if the string represents a shell token divisor.
 *
 * Evaluates the input string to determine if it matches known shell
 * operators or a space. Returns the corresponding token type. If no
 * match is found, returns WORD.
 *
 * @param str  The string to check for being a divisor/operator.
 *
 * @return  The t_token_type representing the divisor (PIPE, REDIR_IN, etc.)
 *          or WORD if the string is not a divisor.
 */
int		is_divisor(char *str);

/**
 * @brief Determines the type of a shell token from its string value.
 *
 * Compares the input string to known shell operators and returns the
 * corresponding token type. If no operator matches, the token is
 * classified as a WORD. Empty or NULL strings return END.
 *
 * @param str  The token string to evaluate.
 *
 * @return  The t_token_type of the string (WORD, PIPE, REDIR_IN, etc.).
 */
int		get_token_type(char *str);

/**
 * @brief Returns the character length of a shell operator token.
 *
 * Determines the length of a token based on its type. Single-character
 * operators (|, <, >) return 1, double-character operators (>>, <<) return 2.
 *
 * @param type 	The token type (t_token_type) to evaluate.
 *
 * @return  Number of characters representing the operator.
 *          Returns 0 if type is not a recognized operator.
 */
int		operator_len(int type);

/**
 * @brief Computes the length of the next word in a string.
 *
 * Scans the input string s until it reaches a divisor character or the end
 * of the string. Handles quoted sections (single or double quotes) as a
 * single word, skipping over internal characters until the closing quote.
 *
 * @param str Input string to measure the word length from.
 *
 * @note  Uses is_divisor() to identify word boundaries.
 * @note  Quotes are handled properly; the length includes all characters
 *        inside the quotes.
 *
 * @return Length of the next word in characters.
 */
int		word_len(const char *str);

/**
 * @brief Frees a partially allocated array of strings.
 *
 * Frees each string in wrdstr up to the specified index, then frees
 * the array itself.
 *
 * @param wrdstr  Array of strings to free.
 * @param index   Number of elements to free from the array.
 *
 * @return Always returns NULL to simplify error handling in callers.
 */
void	*memfree(char **wrdstr, int index);

/**
 * @brief Calculates the visual length of a shell prompt string.
 *
 * Parses the ps1 prompt string and counts the number of characters that
 * will be displayed. Recognizes escape sequences:
 *   - \u : replaced by the username string
 *   - \w : replaced by the current path string
 * Other characters count as one. Backslashes before non-special characters
 * are counted as a single character.
 *
 * @param ps1	Pointer to the prompt string with possible escape sequences.
 * @param user	Pointer to the username string.
 * @param path	Pointer to the current working directory string.
 *
 * @return The total number of characters that will be displayed for prompt.
 */
size_t	prompt_len(char *ps1, char *user, char *path);

/**
 * @brief Expands escape sequences in a shell prompt string.
 *
 * Parses the ps1 string and replaces recognized escape sequences:
 *   - \u : replaced with the username string
 *   - \w : replaced with the current path string
 * Other characters, including non-special backslashes, are copied as-is.
 * The result is written into the pre-allocated tmp buffer.
 *
 * @param tmp 	Pointer to the buffer where the transformed string 
 * 				will be stored.
 * @param ps1 	Pointer to the original prompt string with escape sequences.
 * @param user 	Pointer to the username string.
 * @param path 	Pointer to the current working directory string.
 *
 * @note	The tmp buffer must be large enough to hold the expanded string.
 *
 * @return Pointer to the transformed string stored in tmp.
 */
char	*transform_format(char *tmp, char *ps1, char *user, char *path);

/**
 * @brief Signal handler to refresh the interactive prompt after SIGINT.
 *
 * Sets the global signal flag using set_flag, prints a newline to move
 * the cursor to a new line, clears the current readline buffer, and
 * forces readline to display a fresh prompt.
 *
 * @param signum The received signal number (typically SIGINT).
 *
 * @note	For interactive prompt context only.
 * @note	Uses async-signal-safe functions where applicable.
 * @note	Sets g_signal_received to allow other parts of the shell to
 *			react to the interrupt if necessary.
 */
void	new_prompt(int signum);

/**
 * @brief Sets the global signal flag to indicate an interrupt.
 *
 * Signal handler that sets the g_signal_received flag to 1 when a
 * SIGINT (or other registered signal) is received. Used by the shell
 * to detect abrupt termination and trigger cleanup in non-interactive
 * mode.
 *
 * @param signum The received signal number (ignored in this function).
 *
 * @note	Only sets the global flag; does not perform cleanup itself.
 */
void	set_flag(int signum);

/**
 * @brief	Finds the next operator token in a string, skipping quoted text.
 *
 * Iterates through the given string until a shell operator (e.g. |, <, >, <<,
 * >>) is found outside any quotes. Quoted substrings are fully skipped to
 * avoid false positives.
 *
 * @param str	Input string to search for the next operator.
 *
 * @return	Pointer to the first operator character found, or NULL if no
 *			operator exists outside quotes.
 */
const char	*operator_strchr(const char *str);

/**
 * @brief	Finds the next word delimiter or operator in a string.
 *
 * Scans the input string until a divisor token (space, pipe, or redirection)
 * is found outside of quotes. Quoted substrings are skipped to ensure that
 * delimiters within quotes are ignored.
 *
 * @param str	Input string to scan for the next word boundary.
 *
 * @return	Pointer to the first divisor character found, or NULL if none
 *			exist outside quotes.
 */
const char	*word_strchr(const char *str);

#endif