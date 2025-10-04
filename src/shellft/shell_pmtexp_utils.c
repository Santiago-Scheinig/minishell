/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtexp_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:32:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 20:46:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

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
char	*transform_format(char *tmp, char *ps1, char *user, char *path)
{
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	while (ps1 && ps1[i])
	{
		if (ps1[i] == '\\' && ps1[i + 1] && ps1[i + 1] != '\\')
		{
			if (ps1[i + 1] == 'u')
			{
				ft_memcpy(&tmp[j], user, ft_strlen(user));
				j += ft_strlen(user);
			}
			else if (ps1[i + 1] == 'w')
			{
				ft_memcpy(&tmp[j], path, ft_strlen(path));
				j += ft_strlen(path);
			}
			else
				tmp[j++] = ps1[i + 1];
			i += 2;
		}
		else
			tmp[j++] = ps1[i++];
	}
	tmp[j] = '\0';
	return (tmp);
}

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
size_t	prompt_len(char *ps1, char *user, char *path)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (ps1 && ps1[i])
	{
		if (ps1[i] == '\\' && ps1[i + 1] && ps1[i + 1] != '\\')
		{
			if (ps1[i + 1] == 'u')
				len += ft_strlen(user);
			else if (ps1[i + 1] == 'w')
				len += ft_strlen(path);
			else
				len += 1;
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
