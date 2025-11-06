/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 16:09:00 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 18:23:26 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <curses.h>
#include <term.h>
#include "libft.h"
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

static int	handle_char(char **buffer, int *size, int *pos, char chr)
{
	int	i;

	i = 0;
	buffer[0] = ft_realloc(buffer[0], size[0] + 2, size[0]);
	if (!buffer[0])
		return (false);
	ft_memmove(&buffer[0][pos[0] + 1], &buffer[0][pos[0]], size[0] - pos[0]);
	buffer[0][pos[0]] = chr;
	size[0]++;
	pos[0]++;
	if (write(STDOUT_FILENO, &buffer[0][pos[0] - 1], size[0] - pos[0] + 1))
		return (false);
	while (i < size[0] - pos[0])
	{
		if (write(STDOUT_FILENO, "\b", 1) == -1)
			return (false);
		i++;
	}
	return (true);
}

static int	handle_backspace(char **buffer, int *new_size, int *pos)
{
	int	i;

	i = 0;
	if (pos[0] <= 0)
		return (true);
	ft_memmove(&buffer[0][pos[0] - 1], buffer[0][pos[0]], new_size[0] - pos[0]);
	if (write(STDOUT_FILENO, "\b", 1) == -1)
		return (false);
	if (write(STDOUT_FILENO, &buffer[0][pos[0]], new_size[0] - pos[0]) == -1)
		return (false);
	if (write(STDOUT_FILENO, " ", 1) == -1)
		return (false);
	while(i <= size[0] - pos[0])
	{
		if (write(STDOUT_FILENO, "\b", 1) == -1)
			return (false);
		i++;	
	}
	new_size[0]--;
	pos[0]--;
	return (true);
}

static int	handle_ctrl_d(char **buffer, int *new_size, int *pos)
{
	if (*new_size == 0)
		return (false);
	return (true);
}

static void	move_cursor(int col, int row)
{
	char *cm;
	char *seq;

	cm = tgetstr("cm", NULL);
	if (!cm)
		return;
	seq = tgoto(cm, col, row);
	tputs(seq, 1, putchar);
}

static int	handle_sequence(char **buffer, int *new_size, int *pos)
{
	char	seq[3];

	if (read(STDIN_FILENO, &seq[1], 2) != 2)
		return (false);
	seq[0] = 27;
	if (seq[2] == 'D' && pos[0] > 0)
	{
		pos[0]--;
		move_cursor(pos[0], 0);
	}
	if (seq[2] == 'C' && pos[0] < new_size[0])
	{
		pos[0]++;
		move_cursor(pos[0], 0);
	}
	return (true);
}

int	handlers(char **buffer, int *new_size, int *pos, char chr)
{
	if (chr == 127)
		return (handle_backspace(buffer, new_size, pos));
	if (chr == 4)
		return (handle_ctrl_d(buffer, new_size, pos));
	if (chr == 27)
		return (handle_sequence(buffer, new_size, pos));
	return (handle_char(&buffer, new_size, pos, chr));
}