/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_hdoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:35:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/24 19:39:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	heredoc_mask(char *limit_mask, char **line, t_list *envp, t_hdoc_pair hd)
{
	int	i;
	int	exp;

	i = -1;
	exp = 1;
	while(limit_mask[++i])
		if (limit_mask[i] == 'S' || limit_mask[i] == 'D')
			exp = 0;
	if (exp)
		if (envar_syntax(line, NULL, envp, hd.exit_no))
			return (MSHELL_FAILURE);
	if (write(hd.fd, (*line), ft_strlen((*line))) == -1)
		return(MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

static int	heredoc_end(const char *err_ft, t_hdoc_tsh troubleshoot, t_body *msh)
{
	const char *msg[3] = {
		"msh: warning:",
		"here-document at line",
		"delimited by end-of-file",
	};

	msh->exit_ft = err_ft;
	if (errno)
	{
		free(troubleshoot.limitator);
		close(troubleshoot.heredoc[0]);
		close(troubleshoot.heredoc[1]);
		return (-1);
	}
	ft_printfd(2, "\n%s %s %i %s ", msg[0], msg[1], troubleshoot.line, msg[2]);
	ft_printfd(2, "(wanted '%s')\n", troubleshoot.limitator);
	close(troubleshoot.heredoc[1]);
	return (troubleshoot.heredoc[0]);
}

static t_hdoc_tsh	troubleshoot_dup(char *limitator, int line, int heredoc[2])
{
	t_hdoc_tsh troubleshoot;

	troubleshoot.limitator = limitator;
	troubleshoot.line = line;
	troubleshoot.heredoc[0] = heredoc[0];
	troubleshoot.heredoc[1] = heredoc[1];
	return (troubleshoot);
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
 * written on heredoc[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
int	heredoc_dup(t_token *limit, int heredoc[2], t_body *msh)
{
	int			i;
	char		*line;
	t_hdoc_pair	hd;

	i = 0;
	hd.exit_no = msh->exit_no;
	hd.fd = heredoc[1];
	shell_memmove(limit->str, limit->str, limit->mask, ft_strlen(limit->str)+ 1);
	while (1 && ++i)
	{
		if (write(1, "> ", 2) == -1)
			return (heredoc_end("write", troubleshoot_dup(limit->str, i, heredoc), msh));
		line = get_next_line(STDIN_FILENO);
		if (!line && errno == ENOMEM)
			return (heredoc_end("malloc", troubleshoot_dup(limit->str, i, heredoc), msh));
		else if (!line && errno)
			return (heredoc_end("read", troubleshoot_dup(limit->str, i, heredoc), msh));
		else if (!line)
			return (heredoc_end(NULL, troubleshoot_dup(limit->str, i, heredoc), msh));
		if (line && !ft_strncmp(line, limit->str, ft_strlen(line) - 1))
		{
			free(line);
			break;
		}
		if (heredoc_mask(limit->mask, &line, msh->envp_lst, hd))
		{
			if (errno == ENOMEM)
				return (heredoc_end("malloc", troubleshoot_dup(limit->str, i, heredoc), msh));
			else
				return (heredoc_end("write", troubleshoot_dup(limit->str, i, heredoc), msh));
		}
		free(line);
	}
	close(heredoc[1]);
	return (heredoc[0]);
}
