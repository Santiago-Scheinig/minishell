/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/03 21:42:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into a new pipefd[1].
 * @param limitator The string that will work as LIMITATOR.
 * @return Returns the pipefd[0] from where to read everything that was 
 * written on pipefd[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on pipefd[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
int	fd_heredoc(char *limitator)
{
	char	*line;
	int		pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(0);
		if (!line)
			break;
		if (line && !ft_strncmp(line, limitator, ft_strlen(limitator)))
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

void	fd_setexe()

void	fd_endexe()

void	fd_setold()

void	fd_endold()