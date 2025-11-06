/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 05:12:25 by root              #+#    #+#             */
/*   Updated: 2025/10/10 06:22:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
size_t	strlend(char *str, char end)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != 0 && str[i] != end)
		i++;
	if (!str[i] && str[i] != end)
		return (0);
	return (i);
}

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
char	*checkbin(char *line)
{
	int	c;
	int	i;

	i = -1;
	while (line[++i])
	{
		c = (int) line[i];
		if (c < 0 || c > 255)
			line[i] = ' ';
	}
	return (line);
}

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
void	rewrite(char **txt, int line_len, int end)
{
	if ((*txt) && (*txt)[line_len])
		(*txt) = ft_calloc(end + 1, sizeof(char));
	else
		(*txt) = NULL;
}
