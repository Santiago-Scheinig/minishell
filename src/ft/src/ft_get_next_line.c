/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 05:11:45 by root              #+#    #+#             */
/*   Updated: 2025/10/10 06:06:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

/**
 * @brief	Frees a NULL-terminated array of strings.
 *
 *			Iterates through the array 'txt', freeing each string
 *			and setting its pointer to NULL. Returns NULL for
 *			convenience in assignments.
 *
 * @param	txt		Array of strings to free.
 *
 * @return	NULL always.
 */
static	void	*forcend(char **txt)
{
	int		i;

	i = -1;
	while (txt[++i])
	{
		free(txt[i]);
		txt[i] = NULL;
	}
	return (NULL);
}

/**
 * @brief	Allocates or expands a buffer for reading text.
 *
 *			If 'str' is NULL, allocates a new buffer of size
 *			BUFFER_SIZE + 1. Otherwise, expands the existing buffer
 *			to accommodate BUFFER_SIZE more bytes, preserving the
 *			existing content.
 *
 * @param	str		String buffer to create or expand.
 *
 * @return	Pointer to the new buffer, or NULL on allocation failure.
 */
static	char	*new_text(char *str)
{
	char	*aux;
	int		str_len;

	if (!str)
	{
		str = ft_calloc((BUFFER_SIZE + 1), sizeof(char));
		if (!str)
			return (NULL);
		return (str);
	}
	aux = str;
	str_len = strlend(str, 0);
	str = ft_calloc((str_len + BUFFER_SIZE + 1), sizeof(char));
	if (!str)
	{
		free(aux);
		return (NULL);
	}
	ft_strlcpy(str, aux, str_len + 1);
	free(aux);
	return (str);
}

/**
 * @brief	Reads from file descriptor until a newline or EOF.
 *
 *			Uses 'txt' as the current buffer and reads in chunks
 *			of BUFFER_SIZE until a newline character is found or
 *			the end of the file is reached.
 *
 * @param	txt		Buffer holding current content.
 * @param	fd		File descriptor to read from.
 *
 * @return	Updated buffer with newly read content, or NULL on error.
 */
static	char	*read_text(char *txt, int fd)
{
	int	line;
	int	r_ans;

	line = 0;
	if (txt)
		line = strlend(txt, '\n');
	while (!line)
	{
		txt = new_text(txt);
		if (!txt)
			return (NULL);
		r_ans = read(fd, &txt[strlend(txt, 0)], BUFFER_SIZE);
		if (r_ans < 0)
		{
			free(txt);
			return (NULL);
		}
		if (!r_ans || txt[0] == '\n')
			return (txt);
		line = strlend(txt, '\n');
	}
	return (txt);
}

/**
 * @brief	Extracts a line from the buffer and updates it.
 *
 *			Allocates a new string 'line' containing characters
 *			from the start of '*txt' up to and including the first
 *			newline. Updates '*txt' to remove the returned line.
 *
 * @param	txt		Pointer to the buffer holding file content.
 *
 * @return	Pointer to the extracted line, or NULL on allocation error.
 */
static	char	*line_text(char **txt)
{
	char	*line;
	char	*aux;
	int		line_len;
	int		end;

	line_len = strlend((*txt), '\n') + 1;
	if (line_len == 1)
	{
		if ((*txt)[0] == '\n')
			line_len = 1;
		else
			line_len = strlend((*txt), 0);
	}
	line = ft_calloc((line_len + 1), sizeof(char));
	if (!line)
		return (NULL);
	ft_strlcpy(line, (*txt), line_len + 1);
	aux = (*txt);
	end = strlen(&(*txt)[line_len]);
	rewrite(txt, line_len, end);
	if (end)
		ft_strlcpy((*txt), &aux[line_len], end + 1);
	free(aux);
	return (line);
}

/**
 * @brief	Reads the next line from a file descriptor.
 *
 *			Handles multiple file descriptors using a static array.
 *			Reads content incrementally using BUFFER_SIZE chunks.
 *			Returns the next line including the newline character if
 *			present.
 *
 * @param	fd		File descriptor to read from.
 *
 * @note	Supports multiple file descriptors simultaneously.
 * @note	Returns NULL on EOF or error.
 *
 * @return	Next line read from the file, or NULL if none.
 */
char	*ft_get_next_line(int fd)
{
	static char	*txt[1000];
	char		*line;

	errno = 0;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	txt[fd] = read_text(txt[fd], fd);
	if (!txt[fd])
		return (forcend(txt));
	if (!txt[fd][0])
	{
		free(txt[fd]);
		txt[fd] = NULL;
		line = NULL;
	}
	else
		line = line_text(&txt[fd]);
	if (!line && txt[fd])
		return (forcend(txt));
	return (line);
}
