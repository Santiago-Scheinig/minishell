/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/08 21:39:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
t_list	*cmdupd_err(t_list *lst_t_token, t_cmd **new_cmd)
{
	t_token	*aux_tkn;

	if ((*new_cmd)->argv)
		ft_split_free((*new_cmd)->argv);
	if ((*new_cmd)->pathname)
		free((*new_cmd)->pathname);
	memset((*new_cmd), 0, sizeof(t_cmd));
	while (lst_t_token->next)
	{
		aux_tkn = (t_token *) lst_t_token->next->content;
		if (aux_tkn->type == PIPE)
			return (lst_t_token);
		lst_t_token = lst_t_token->next;
	}
	return (lst_t_token);
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
		return (shell_redirerr(next->str, MSHELL_FAILURE));
	}
	else
	{
		new->infd = open(next->str, O_RDONLY);
		if (new->infd < 0)
			return (redirend(next->str, MSHELL_FAILURE));
	}
	free(next->str);
	next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
static int	cmdupd_outfile(int open_flag, t_token *next, t_cmd *new)
{
	if (new->outfd > 2)
		close(new->outfd);
	new->outfd = open(next->str, O_WRONLY | O_CREAT | open_flag, 0664);
	if (new->outfd < 0)
		return (redirend(next->str, MSHELL_FAILURE));
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
	if (aux->heredoc_fd == -1)
		return (MSHELL_FAILURE);
	new->infd = aux->heredoc_fd;
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT PENDING
 */
int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		return (cmdupd_infile(aux_next, new));
	if (aux->type == REDIR_OUT)
		return (cmdupd_outfile(O_TRUNC, aux_next, new));
	if (aux->type == REDIR_APPEND)
		return (cmdupd_outfile(O_APPEND, aux_next, new));
	if (aux->type == HEREDOC)
		return (cmdupd_heredoc(aux, new));
	return (MSHELL_FAILURE);
}
