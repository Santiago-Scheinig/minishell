/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:23:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:57:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Writes a string followed by a newline to a file descriptor.
 *
 * 			Sends the string 's' to the file or output stream associated
 *			with 'fd', then writes a newline character.
 *
 * @param	s	String to write.
 * @param	fd	File descriptor where the string will be written.
 *
 * @return	Total number of bytes written, or -1 on error.
 */
int	ft_putendl_fd(char *s, int fd)
{
	int	w_bytes;

	w_bytes = ft_putstr_fd(s, fd);
	w_bytes += ft_putchar_fd('\n', fd);
	return (w_bytes);
}
