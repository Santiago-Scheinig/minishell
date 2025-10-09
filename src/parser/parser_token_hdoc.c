/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_hdoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:35:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/08 21:46:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	heredoc_mask(char *mask, char **line, t_list *envp, t_hdoc_pair hd)
{
	int	i;
	int	exp;

	i = -1;
	exp = 1;
	while (mask[++i])
		if (mask[i] == 'S' || mask[i] == 'D')
			exp = 0;
	if (exp)
		if (envar_syntax(line, NULL, envp, hd.exit_no))
			return (MSHELL_FAILURE);
	if (write(hd.fd, (*line), ft_strlen((*line))) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

static int	hdoc_end(char *limit, char *err_ft, int heredoc_fd[2], t_body *msh)
{
	const char	*msg[3] = {
		"msh: warning:",
		"here-document at line",
		"delimited by end-of-file",
	};

	if (errno == ENOMEM)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	else if (errno)
		msh->exit_ft = err_ft;// print write or read errors
	else
	{
		ft_printfd(2, "\n%s %s %i %s ", msg[0], msg[1], msh->line, msg[2]);
		ft_printfd(2, "(wanted '%s')\n", limit);
		close(heredoc_fd[1]);
		return (heredoc_fd[0]);
	}
	free(limit);
	close(heredoc_fd[0]);
	close(heredoc_fd[2]);
	return (MSHELL_FAILURE);
}

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into heredoc[1].
 * 
 * @param limitator The string that will work as LIMITATOR.
 * @param heredoc An array of INT which saves an already initialized pipe()
 * @return Returns heredoc[0] from where to read everything that was 
 * written on heredoc[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on heredoc[0] will be sent to the next cmd and an error msg is 
 * printed on STDERR specifying the interruption issue.
 */
int	heredoc_dup(t_token *limit, int heredoc_fd[2], t_body *msh)
{
	int			strlen;
	char		*line;
	t_hdoc_pair	hd_utils;

	hd_utils.fd = heredoc_fd[1];
	hd_utils.exit_no = msh->exit_no;
	strlen = ft_strlen(limit->str) + 1;
	shell_memmove(limit->str, limit->str, limit->mask, strlen);
	while (1 && ++msh->line)
	{
		if (msh->interactive && write(1, "> ", 2) == -1)
			return (hdoc_end(limit->str, "write", heredoc_fd, msh));
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (hdoc_end(limit->str, "read", heredoc_fd, msh));
		if (line && ft_strlen(line) != 1 
			&& !ft_strncmp(line, limit->str, ft_strlen(line) - 1))
			break ;
		if (heredoc_mask(limit->mask, &line, msh->lst_t_var, hd_utils))
			return (hdoc_end(limit->str, "read", heredoc_fd, msh));
		free(line);
	}
	free(line);
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}
