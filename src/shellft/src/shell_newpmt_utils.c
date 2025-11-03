/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newpmt_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:32:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/18 17:58:02 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * Need fix
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
