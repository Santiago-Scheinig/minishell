/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 13:27:57 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 13:47:13 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*create_buffer(void)
{
	char	*buffer;
	char	c;
	int		i;

	while (1)
	{
		i = read(STDIN_FILENO, &c, 1);
		if (i <= 0 || c == '\n')
			break ;
		buffer = ft_realloc()
	}
	
}

char	*ft_readline(const char *prompt)
{
	char	*buffer;

	if (write(STDOUT_FILENO, prompt, ft_strlen(prompt)) == -1)
		//error write;
	buffer = create_buffer();
	if (!buffer)
		//error malloc;
	return (buffer);
}