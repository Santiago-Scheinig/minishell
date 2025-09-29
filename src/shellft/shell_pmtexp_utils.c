/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtexp_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:32:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/29 12:19:42 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * Transforms a PS1-style format string into a concrete prompt string.
 * 
 * @param tmp  Buffer where the transformed prompt will be written.
 * @param ps1  Format string using backslash escapes.
 * @param user Username string to substitute for "\u".
 * @param path Current working directory string to substitute for "\w".
 * 
 * Iterates ps1 and copies characters into tmp. Recognizes backslash escapes:
 *  - "\u" -> inserts the user string
 *  - "\w" -> inserts the path string
 *  - any other backslash escape inserts the next character verbatim
 * Consecutive backslashes ("\\") are treated as literal backslashes.
 * 
 * @return Pointer to tmp.
 * @note tmp must be large enough to hold the resulting string.
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
 * Calculates the length of the prompt that will be produced from a PS1 format.
 * 
 * @param ps1  Format string using backslash escapes.
 * @param user Username string to substitute for "\u".
 * @param path Current working directory string to substitute for "\w".
 * 
 * @return The number of characters required for the transformed prompt.
 * @note Use this value to allocate a buffer for transform_format.
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
