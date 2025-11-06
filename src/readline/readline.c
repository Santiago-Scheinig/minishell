/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:27:57 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 17:55:46 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static int	disable_term_mode(struct termios *old_term)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, old_term) == -1)
		return (false);
	return (true);
}

static int	set_term_mode(struct termios *old_term)
{
	struct termios	new_term;

	if (tcgetattr(STDIN_FILENO, old_term) == -1)
		return (false);
	new_term = *old_term;
	new_term.c_lflag &= ~(ICANON | ECHO);
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0; 
	if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) == -1)
		return (false);
	return (true);
}

static char	*create_buffer(void)
{
	char	*buffer;
	char	chr;
	int		new_size;
	int		pos;
	int		i;

	buffer = NULL;
	new_size = 0;
	pos = 0;
	while (1)
	{
		i = read(STDIN_FILENO, &chr, 1);
		if (i <= 0 || chr == '\n')
			break ;
		if (!handlers(&buffer, &new_size, &pos, chr))
		{
			free(buffer);
			buffer = NULL;
			return (NULL);
		}
	}
	if (buffer)
    	buffer[pos] = '\0';
	return (buffer);
}

char	*ft_readline(const char *prompt)
{
	struct termios	old_term;
	char			*buffer;

	if (!set_term_mode(&old_term))
		return (NULL);
	if (write(STDOUT_FILENO, prompt, ft_strlen(prompt)) == -1)
	{
		disable_term_mode(&old_term);
		return (NULL);
	}
	buffer = create_buffer();
	disable_term_mode(&old_term);
	return (buffer);
}