/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:38:00 by sscheini          #+#    #+#             */
/*   Updated: 2025/06/09 15:01:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
static int	ft_read_to_limitator(char *limitator)
{
	char	*line;
	int		pipefd[2];
	int		limitator_len;

	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(0);
		limitator_len = ft_strlen(limitator);
		if (line && !ft_strncmp(line, limitator, limitator_len))
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

/**
 * Executes and pipes every command specified on the program enviroment,
 * but the first command will read from the STDIN instead of the infile 
 * (until the LIMITATOR is writen).
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
int	ft_do_here_doc(t_pipex *env, char **envp, char *limitator)
{
	int	i;
	int	infd;
	int	pipefd[2];

	i = -1;
	env->waitpid_list = ft_calloc((env->cmd_count), sizeof(pid_t));
	if (!env->waitpid_list)
		return (-1);
	while (++i < env->cmd_count)
	{
		if (!i)
			infd = ft_read_to_limitator(limitator);
		if (infd < 0 || pipe(pipefd) == -1)
			return (ft_waitfor_childs(env, EXIT_FAILURE));
		if (i == env->cmd_count - 1)
		{
			close(pipefd[1]);
			pipefd[1] = open(env->outfile, O_CREAT | O_APPEND | O_WRONLY, 420);
			if (pipefd[1] < 0)
				return (ft_waitfor_childs(env, EXIT_FAILURE));
		}
		infd = ft_do_fork(env, infd, pipefd, envp);
	}
	return (ft_waitfor_childs(env, EXIT_SUCCESS));
}
