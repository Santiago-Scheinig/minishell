/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newpmt_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:32:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/12 17:44:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

static int	replace(char *tmp, char *value, int size)
{
	ft_memcpy(tmp, value, size);
	return (ft_strlen(value));
}

/**
 * @brief	Transforms a prompt string replacing placeholders with values.
 *
 * 			Parses the PS1 string and replaces escape sequences with
 *			actual values: '\u' is replaced by the username, '\w' by the
 *			current path. Other characters after '\' are copied literally.
 *
 * @param	tmp		Pre-allocated buffer to store the transformed string.
 * @param	ps1		Original prompt format string containing placeholders.
 * @param	user	String representing the username for '\u' replacement.
 * @param	path	String representing the path for '\w' replacement.
 *
 * @return	Pointer to the buffer tmp containing the transformed prompt.
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
				j += replace(&tmp[j], user, ft_strlen(user));
			else if (ps1[i + 1] == 'w')
				j += replace(&tmp[j], path, ft_strlen(path));
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
