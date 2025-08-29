/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/29 19:15:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"
#include "parser.h"

static t_cmd	*cmd_init(t_list *token_lst)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	memset(new_cmd, 0, sizeof(t_cmd));
	new_cmd->outfile = 1;
	new_cmd->heredoc[0] = -1;
	new_cmd->heredoc[1] = -1;
	argc = cmd_argc(token_lst) + 1;
	new_cmd->argv = malloc((argc) * sizeof(char *));
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	memset(new_cmd->argv, 0, (argc * sizeof(char *)));
	return (new_cmd);
}

static int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		return(edit_infile(aux_next, new));
	if (aux->type == REDIR_OUT)
		return (edit_outfile(aux_next, new, O_TRUNC));
	if (aux->type == REDIR_APPEND)
		return (edit_outfile(aux_next, new, O_APPEND));
	if (aux->type == HEREDOC)
		return (edit_infile_to_heredoc(aux_next, new));
	return (MSHELL_FAILURE)
}

static void	cmdupd_argv(t_token *aux, t_cmd *new_cmd, t_body *minishell)
{
	int		i;

	i = 0;
	while (new->argv[i])
		i++;
	new->argv[i] = shell_substr(aux->str, aux->mask, 0, ft_strlen(aux->str));
	if (!new->argv[i])
		forcend(minishell, "malloc", MSHELL_FAILURE);
	if (aux->str)
		free(aux->str);
	aux->str = NULL;
}

static void	cmdupd_save( t_list *token_lst, t_cmd **new_cmd, t_body *minishell)
{
	t_list	*new_node;

	if ((*new_cmd))
	{
		new_node = ft_lstnew((*new_cmd));
		if (!new_node)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		ft_lstadd_back(&(minishell->cmd_lst), new_node);
	}
	if (token_lst)
	{
		(*new_cmd) = cmd_init(token_lst->next, minishell);
		if (!(*new_cmd))
			forcend(minishell, "malloc", MSHELL_FAILURE);
	}
}

void	parser_cmds(t_body *minishell)
{
	t_cmd	*new_cmd;
	t_list	*aux_lst;
	t_token	*aux_tkn;

	aux_lst = minishell->token_lst;
	new_cmd = cmd_init(minishell->token_lst);
	if (!new_cmd)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	while (aux_lst)
	{
		aux_tkn = (t_token *) token_lst->content;
		if (aux_tkn->type && aux_tkn->type != PIPE && aux_tkn->type != END)
		{
			aux_lst = aux_lst->next;
			if (cmdupd_redir(aux_tkn, (t_token *) aux_lst->content, new_cmd))
				aux_lst = cmd_rerr(aux_lst, &new_cmd);
		}
		else if (aux_tkn->type == WORD)
			cmdupd_argv(aux_tkn, new_cmd);
		else if (aux_tkn->type == PIPE)
			cmdupd_save(aux_lst, new_cmd, minishell);
		aux_lst = aux_lst->next;
	}
	cmdupd_save(NULL, new_cmd, minishell);
}
