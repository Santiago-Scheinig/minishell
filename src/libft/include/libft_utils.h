/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 05:23:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:22:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_UTI_H
# define LIBFT_UTI_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

#include "libft.h"

/*--------------------------------------------------------------------------*/
/*-----------------------------------SPLIT----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Validates that no string in 'base' contains a space.
 *
 *			Iterates through each string in the 'base' array. If any
 *			string contains a space character, the function returns
 *			false (0). Otherwise, returns true (1) if all strings are
 *			valid.
 *
 * @param	base	Array of strings to validate.
 *
 * @return	1 if all strings are valid, 0 otherwise.
 */
int			isset_valid(const char **base);

/**
 * @brief	Checks if the start of 's' matches any string in 'base'.
 *
 *			Iterates through each string in the 'base' array. If the
 *			beginning of 's' matches a base string exactly, returns its
 *			length. Otherwise, returns false (0).
 *
 * @param	s		String to check for a base match.
 * @param	base	Array of strings representing the base set.
 *
 * @return	Length of the matching base string, or false (0) if none.
 */
int			isset(const char *s, const char **base);

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
 * @brief	Calculates the length of a word, considering quotes and sets.
 *
 *			Parses the string 's' until the delimiter 'c' or a character
 *			found in 'base' is reached. Quoted substrings are skipped
 *			as single units, so delimiters inside quotes are ignored.
 *
 * @param	s		String to measure the word length from.
 * @param	c		Delimiter character that ends the word.
 * @param	base	Array of strings representing character sets to skip.
 *
 * @return	Length of the word, accounting for quotes and base sets.
 */
int			wrdlen_iqreg(const char *s, char c, const char **base);


/*--------------------------------------------------------------------------*/
/*----------------------------------PRINTF----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Formats a decimal string with width, precision, and sign flags.
 *
 *			Handles precision zero with value zero by outputting an empty
 *			string or "+" if flags[1] is set. Sets sign for '+' or ' ' flags
 *			using setsign(). Applies precision using getpsc() if needed.
 *			Handles zero-padding when precision is unset and width exceeds
 *			string length. Finally applies width adjustment with txtwth().
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	str		Decimal string to format.
 * @param	str_len	Length of the decimal string.
 *
 * @note	Allocates memory for new formatted strings as needed.
 * @note	May free the original str during processing.
 *
 * @return	Pointer to formatted decimal string, or NULL on failure.
 */
char		*dgtwth(int *flags, char *str, int str_len);

/**
 * @brief	Parses format flags, width, precision, and specifier for printf.
 *
 *			Analyzes the string 's' for standard printf-style flags,
 *			width, precision, and conversion specifiers. Returns an
 *			array representing the parsed values.
 *
 *			Static helpers:
 *
 *				- isspecifier():	Checks if a character is a valid format
 *									specifier.
 *
 *				- flagcheck():		Sets corresponding flags in the array
 *									for '-', '+', ' ', '#', and '0'.
 *
 *				- widthcheck():		Parses field width and stores in array.
 *
 *				- precisioncheck(): Parses precision after '.' and stores
 *									it in the array.
 *
 * @param	s	String containing format specifiers (e.g., "-10.3d").
 *
 * @note	Allocates memory for the flags array using ft_calloc.
 * @note	Flags array structure: [minus, plus, space, hash, zero, width,
 *			precision, specifier].
 *
 * @return	Pointer to an int array with flags, width, precision, and
 *			specifier. Returns NULL on allocation failure or invalid spec.
 */
int			*flagdescriptor(const char *s);

/**
 * @brief	Creates a new string containing a single character.
 *
 *			Allocates memory for a 2-character string: the given
 *			character and a null terminator.
 *
 * @param	c	Character to duplicate into a new string.
 *
 * @return	Pointer to the newly allocated string, or NULL if allocation fails.
 */
char		*ft_chrdup(char c);

/**
 * @brief	Prefixes a string pointer with "0x" or "0X" for hex output.
 *
 *			Used to format pointer or hexadecimal strings. Frees the
 *			original string after creating the new prefixed string.
 *
 * @param	ptr		String representing a pointer or hex number. Will be freed.
 * @param	spf		Specifier character ('x' for lowercase, 'X' for uppercase).
 *
 * @return	New string prefixed with "0x" or "0X", or NULL on allocation failure.
 */
char		*ft_ptrdup(void *ptr, int spf);

/**
 * @brief	Selects the hexadecimal base string based on the specifier.
 *
 *			Returns a pointer to the character set used for converting
 *			numbers to hexadecimal strings. Lowercase is used for 'x'
 *			and 'p', uppercase for 'X'.
 *
 * @param	specifier	Character indicating the desired hex format ('x', 'X', 'p').
 *
 * @return	Pointer to a string containing hexadecimal digits in the
 *			correct case, or NULL if the specifier is invalid.
 */
const char	*ft_sethexa(int specifier);

/**
 * @brief	Applies width, precision, and prefix to a hexadecimal string.
 *
 *			Handles '#' flag by prepending "0x" or "0X" for non-zero values.
 *			Applies precision using getpsc() if flags[6] exceeds string len.
 *			Handles zero-padding with '0' flag when precision is unset.
 *			Finally, adjusts the string width using txtwth().
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	str		Hexadecimal string to format.
 * @param	str_len	Length of the hexadecimal string.
 *
 * @note	Allocates memory for new formatted strings as needed.
 * @note	May free the original str during processing.
 *
 * @return	Pointer to formatted hexadecimal string, or NULL on failure.
 */
char		*hexwth(int *flags, char *str, int str_len);

/**
 * @brief	Applies field width formatting to a string.
 *
 *			Generates a new string of length equal to the field width
 *			specified in 'flags'. Pads the original string 'str' with
 *			spaces either to the left or right depending on the '-' flag.
 *
 * @param	flags	Array containing printf-like formatting information.
 *					flags[0] is the left-justify flag, flags[5] is width,
 *					flags[7] is the specifier.
 * @param	str		Original string to format.
 * @param	str_len	Length of the original string.
 *
 * @note	Allocates a new string with ft_calloc. Frees the original string.
 * @note	If width <= str_len or specifier is '%', returns a substring copy
 *			of 'str'.
 *
 * @return	Pointer to the newly formatted string, or NULL on allocation
 *			failure.
 */
char		*txtwth(int *flags, char *str, int str_len);

/*--------------------------------------------------------------------------*/
/*------------------------------GET_NEXT_LINE-------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Replaces non-ASCII characters in a string with spaces.
 *
 *			Iterates through the input string and checks the ASCII value
 *			of each character. Any character with a value less than 0 or
 *			greater than 255 is replaced by a space.
 *
 * @param	line	String to check and modify in-place.
 *
 * @note	The function modifies the original string.
 *
 * @return	Pointer to the modified input string.
 */
char		*checkbin(char *line);

/**
 * @brief	Calculates the length of a string up to a given end character.
 *
 *			Iterates through the string until it finds either the null
 *			terminator or the specified end character. Returns 0 if the
 *			string is NULL or if the end character is not found.
 *
 * @param	str		String to measure.
 * @param	end		Character at which to stop measuring.
 *
 * @note	If the string is empty or the end character is not found,
 *			the function returns 0.
 *
 * @return	Length of the string up to 'end', or 0 if not found/NULL.
 */
size_t		strlend(char *str, char end);

/**
 * @brief	Allocates or resets a string buffer based on conditions.
 *
 *			If the string exists and the character at line_len is not
 *			the null terminator, allocates a new buffer of size end + 1.
 *			Otherwise, sets the pointer to NULL.
 *
 * @param	txt		Pointer to the string pointer to modify.
 * @param	line_len	Index to check within the string.
 * @param	end		Size of the new buffer to allocate if condition met.
 *
 * @note	The original string is not freed inside this function.
 */
void		rewrite(char **txt, int line_len, int end);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif