/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/26 18:26:00 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	edit_infile(t_token *next, t_cmd *new)
{
	if (new->infile && new->infile > 2)
		close(new->infile);
	if (access(next->str, R_OK | F_OK))
	{
		new->infile = -1;
		strerror(0);
	}
	else
	{
		new->infile = open(next->str, O_RDONLY);
		if (new->infile < 0)
			strerror(0);
		if (new->heredoc[0] >= 0)
			close(new->heredoc[0]);
		if (new->heredoc[1] >= 0)
			close(new->heredoc[1]);
		new->heredoc[0] = -1;
		new->heredoc[1] = -1;
	}
	free(next->str);
	next->str = NULL;
}

static void	edit_outfile(t_token *next, t_cmd *new)
{
	if (new->outfile && new->outfile > 2)
		close(new->outfile);
	if (!access(next->str, F_OK))
	{
		if (access(next->str, W_OK))
		{
			new->outfile = -1;
			strerror(0);
		}
	}
	else
	{
		new->outfile = open(next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (new->outfile < 0)
			strerror(0);
	}
	free(next->str);
	next->str = NULL;
}

static void	edit_outfile_to_append(t_token *next, t_cmd *new)
{
	if (new->outfile != 1)
		close(new->outfile);
	if (!access(next->str, F_OK))
	{
		if (access(next->str, W_OK))
		{
			new->outfile = -1;
			strerror(0);
		}
	}
	else
	{
		new->outfile = open(next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (new->outfile < 0)
			strerror(0);
	}
	free(next->str);
	next->str = NULL;
}

static void	edit_infile_to_heredoc(t_token *next, t_cmd *new)
{
	if (new->infile)
	{
		close(new->infile);
		new->infile = -2;
	}
	if (new->heredoc[0] >= 0)
		close(new->heredoc[0]);
	if (new->heredoc[1] >= 0)
		close(new->heredoc[1]);
	if (pipe(new->heredoc) < 0)
		strerror(0);
	if (new->limitator)
		free(new->limitator);
	new->limitator = next->str;
	next->str = NULL;
}

// -1 on either outfile or infile should skip that cmd execution.
void	cmd_redirupd(t_token *aux, t_token *next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		edit_infile(next, new);
	if (aux->type == REDIR_OUT)
		edit_outfile(next, new);
	if (aux->type == REDIR_APPEND)
		edit_outfile_to_append(next, new);
	if (aux->type == HEREDOC)
		edit_infile_to_heredoc(next, new);
}
