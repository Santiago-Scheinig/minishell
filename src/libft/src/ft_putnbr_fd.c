/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 16:28:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:57:41 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Writes an integer to a given file descriptor.
 *
 * 			Sends the integer 'n' as a sequence of characters to the file
 *			or output stream associated with 'fd'. Handles negative numbers
 *			and the special case of INT_MIN.
 *
 * @param	n	Integer to write.
 * @param	fd	File descriptor where the number will be written.
 *
 * @return	Total number of bytes written, or -1 on error.
 */
int	ft_putnbr_fd(int n, int fd)
{
	int	w_bytes;

	w_bytes = 0;
	if (n == INT_MIN)
	{
		w_bytes += ft_putnbr_fd(INT_MIN / 10, fd);
		n = 8;
	}
	if (n < 0)
	{
		n *= -1;
		w_bytes += ft_putchar_fd('-', fd);
	}
	if (n >= 10)
	{
		w_bytes += ft_putnbr_fd(n / 10, fd);
		n = n % 10;
	}
	w_bytes += ft_putchar_fd(n + 48, fd);
	return (w_bytes);
}
