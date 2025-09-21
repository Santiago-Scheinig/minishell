/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/21 19:27:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
t_list	*cmdupd_err(t_list *aux_lst, t_cmd **new_cmd)
{
	t_token *aux_tkn;

	if ((*new_cmd)->argv)
		ft_split_free((*new_cmd)->argv);
	if ((*new_cmd)->pathname)
		free((*new_cmd)->pathname);
	memset((*new_cmd), 0, sizeof(t_cmd));
	while(aux_lst->next)
	{
		aux_tkn = (t_token *) aux_lst->next->content;
		if (aux_tkn->type == PIPE)
			return (aux_lst);
		aux_lst = aux_lst->next;
	}
	return (aux_lst);
}

/**
 * COMMENT PENDING
 */
static int	cmdupd_infile(t_token *next, t_cmd *new)
{
	if (new->infd > 2)
		close(new->infd);
	if (access(next->str, R_OK | F_OK))
	{
		new->infd = -1;
		return(redirend(next->str, MSHELL_FAILURE));
	}
	else
	{
		new->infd = open(next->str, O_RDONLY);
		if (new->infd < 0)
			return(redirend(next->str, MSHELL_FAILURE));
	}
	free(next->str);
	next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
static int	cmdupd_outfile(t_token *next, t_cmd *new, int open_flag)
{
	if (new->outfd > 2)
		close(new->outfd);
	if (!access(next->str, F_OK))
		if (access(next->str, W_OK))
			return(redirend(next->str, MSHELL_FAILURE));
	new->outfd = open(next->str, O_WRONLY | O_CREAT | open_flag, 0644);
	if (new->outfd < 0)
		return(redirend(next->str, MSHELL_FAILURE));
	free(next->str);
	next->str = NULL;
	return (MSHELL_SUCCESS);
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
static int	fd_heredoc(char *limitator, int heredoc[2])
{
	int		i;
	char	*line;

	i = 0;//If limitator is between "" or '' as usual, if not, variables with $ should expand
	while (1)
	{
		i++;
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
		{
			if (errno == ENOMEM)//forcend should check on errno instead of following msg to know what to print?
				return (-1);//i also need to verify if read failed and print a message accordingly
			ft_printfd(2, "\nmsh: warning: here-document at line %i delimited by end-of-file (wanted '%s')\n", i, limitator);
			break;
		}
		if (line && !ft_strncmp(line, limitator, ft_strlen(limitator)))
		{
			free(line);
			break;
		}
		write(heredoc[1], line, ft_strlen(line));//write can fail! line x where x is the line its writing of heredoc
		free(line);
	}
	close(heredoc[1]);//How could i add the heredoc to the history??
	return (heredoc[0]);
}

/**
 * COMMENT PENDING
 */
static int	cmdupd_heredoc(t_token *next, t_cmd *new)
{
	int	heredoc[2];

	if (new->infd > 2)
		close(new->infd);
	if (pipe(heredoc) < 0)
		return(redirend(NULL, MSHELL_FAILURE));
	new->infd = fd_heredoc(next->str, heredoc);//next->str should go through memmove first so quotes are removed
	next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		return(cmdupd_infile(aux_next, new));
	if (aux->type == REDIR_OUT)
		return (cmdupd_outfile(aux_next, new, O_TRUNC));
	if (aux->type == REDIR_APPEND)
		return (cmdupd_outfile(aux_next, new, O_APPEND));
	if (aux->type == HEREDOC)
		return (cmdupd_heredoc(aux_next, new));
	return (MSHELL_FAILURE);
}