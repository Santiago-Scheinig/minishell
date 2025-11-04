/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 14:10:37 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:56:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Writes a character to a given file descriptor.
 *
 * 			Sends the character 'c' to the file or output stream
 *			associated with the file descriptor 'fd'.
 *
 * @param	c	Character to write.
 * @param	fd	File descriptor where the character will be written.
 *
 * @return	Number of bytes written, or -1 on error.
 */
int	ft_putchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}
