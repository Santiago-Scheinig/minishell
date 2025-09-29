/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/29 14:18:27 by sscheini         ###   ########.fr       */
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
 * COMMENT PENDING
 */
static int	cmdupd_heredoc(t_token *aux, t_cmd *new)
{
	if (new->infd > 2)
		close(new->infd);
	if (aux->heredoc == -1)
		return(MSHELL_FAILURE);
	new->infd = aux->heredoc;
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
		return (cmdupd_heredoc(aux, new));
	return (MSHELL_FAILURE);
}
