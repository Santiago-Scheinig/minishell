/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/02 21:06:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "troubleshoot.h"

/**
 * COMMENT PENDING
 */
t_list	*cmd_rerr(t_list *aux_lst, t_cmd **new_cmd)
{
	t_token *aux_tkn;

	if ((*new_cmd)->argv)
		ft_split_free((*new_cmd)->argv);
	if ((*new_cmd)->limitator)
		free((*new_cmd)->limitator);
	if ((*new_cmd)->pathname)
		free((*new_cmd)->pathname);
	if ((*new_cmd))
		free((*new_cmd));
	(*new_cmd) = NULL;
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
int	cmd_argc(t_list *token_lst)
{
	t_token	*aux;
	int		prev_type;
	int		count;

	count = 0;
	prev_type = WORD;
	while (token_lst)
	{
		aux = (t_token *) token_lst->content;
		if (aux->type == PIPE)
			break;
		if (aux->type == WORD && prev_type == WORD)
			count++;
		prev_type = aux->type;
		token_lst = token_lst->next;
	}
	return (count);
}

/**
 * COMMENT PENDING
 */
int	edit_infile(t_token *next, t_cmd *new)
{
	if (new->infile && new->infile > 2)
		close(new->infile);
	if (access(next->str, R_OK | F_OK))
	{
		new->infile = -1;
		return(redirend(next->str, MSHELL_FAILURE));
	}
	else
	{
		new->infile = open(next->str, O_RDONLY);
		if (new->infile < 0)
			return(redirend(next->str, MSHELL_FAILURE));
		if (new->heredoc[0] >= 0)
			close(new->heredoc[0]);
		if (new->heredoc[1] >= 0)
			close(new->heredoc[1]);
		new->heredoc[0] = -1;
		new->heredoc[1] = -1;
	}
	free(next->str);
	next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
int	edit_outfile(t_token *next, t_cmd *new, int open_flag)
{
	if (new->outfile && new->outfile > 2)
		close(new->outfile);
	if (!access(next->str, F_OK))
		if (access(next->str, W_OK))
			return(redirend(next->str, MSHELL_FAILURE));
	new->outfile = open(next->str, O_WRONLY | O_CREAT | open_flag, 0644);
	if (new->outfile < 0)
		return(redirend(next->str, MSHELL_FAILURE));
	free(next->str);
	next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
int	edit_infile_to_heredoc(t_token *next, t_cmd *new)
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
		return(redirend(NULL, MSHELL_FAILURE));
	if (new->limitator)
		free(new->limitator);
	new->limitator = next->str;
	next->str = NULL;
	return (MSHELL_SUCCESS);
}
