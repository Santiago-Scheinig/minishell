/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/29 21:06:01 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void	edit_infile(t_token *aux, t_token *next, t_cmd *new)
{
	if (new->infile && new->infile > 2)
		close(new->infile);
	new->infile = open(next.str, O_RDONLY);
	if (new->infile < 0)
		strerror();
	free(next->str);
	next->str = NULL;
}

static void	edit_outfile(t_token *aux, t_token *next, t_cmd *new)
{
	if (new->outfile && new->outfile > 2)
		close(new->outfile);
	new->outfile = open(next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new->outfile < 0)
		strerror();
	free(next->str);
	next->str = NULL;
}

static void	edit_outfile_to_append(t_token *aux, t_token *next, t_cmd *new)
{
	if (new->outfile != 1)
		close(new->outfile);
	new->outfile = open(next->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (new->outfile < 0)
		sterror();
	free(next.str);
	next->str = NULL;
}

static void	edit_infile_to_heredoc(t_token *aux, t_token *next, t_cmd *new)
{
	if (new->infile)
		close(new->infile);
	if (new->heredoc[0] >= 0)
		close(new->heredoc[0]);
	if (new->heredoc[1] >= 0)
		close(new->heredoc[1]);
	if (pipe(new->heredoc) < 0)
		strerror();
	if (new->limitator)
		free(new->limitator);
	new->limitator = next->str;
}

// -1 on either outfile or infile should skip that cmd execution.
void	update_redir(t_token *aux, t_token *next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		edit_infile(aux, next, new);
	if (aux->type == REDIR_OUT)
		edit_outfile(aux, next, new);
	if (aux->type == REDIR_APPEND)
		edit_outfile_to_append(aux, next, new);
	if (aux->type == HEREDOC)
		edit_infile_to_heredoc(aux, next, new);
}