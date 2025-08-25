/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shellft.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:39:28 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 17:44:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELLFT_H
# define SHELLFT_H

# include "libft.h"

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
char	*shell_substr(char const *s, unsigned int start, size_t len);

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

#endif