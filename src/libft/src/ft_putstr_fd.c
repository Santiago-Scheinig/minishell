/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:21:10 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:57:58 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Writes a string to a given file descriptor.
 *
 * 			Sends the string 's' to the file or output stream associated
 *			with the file descriptor 'fd'.
 *
 * @param	s	String to write.
 * @param	fd	File descriptor where the string will be written.
 *
 * @return	Number of bytes written, or -1 on error.
 */
int	ft_putstr_fd(char *s, int fd)
{
	return (write(fd, s, ft_strlen(s)));
}
