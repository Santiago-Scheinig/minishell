/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/23 18:48:43 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
static int	cmd_argc(t_list *token_lst)
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
static t_cmd	*cmd_init(t_list *token_lst)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	memset(new_cmd, 0, sizeof(t_cmd));
	new_cmd->infd = STDIN_FILENO;
	new_cmd->outfd = STDOUT_FILENO;
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

/**
 * COMMENT PENDING
 */
static void	cmdupd_argv(t_token *aux, t_cmd *new_cmd)
{
	int		i;

	i = 0;
	while (new_cmd->argv[i])
		i++;
	shell_memmove(aux->str, aux->str, aux->mask, ft_strlen(aux->str) + 1);
	new_cmd->argv[i] = aux->str;
	aux->str = NULL;
}

/**
 * COMMENT PENDING
 */
static void	cmdupd_save( t_list *token_lst, t_cmd **new_cmd, t_body *minishell)
{
	t_list	*new_node;

	new_node = ft_lstnew((*new_cmd));
	if (!new_node)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	ft_lstadd_back(&(minishell->cmd_lst), new_node);
	if (token_lst)
	{
		(*new_cmd) = cmd_init(token_lst->next);
		if (!(*new_cmd))
			forcend(minishell, "malloc", MSHELL_FAILURE);
	}
}

/**
 * COMMENT PENDING
 */
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
		aux_tkn = (t_token *) aux_lst->content;
		if (aux_tkn->type && aux_tkn->type != PIPE && aux_tkn->type != END)
		{
			aux_lst = aux_lst->next;
			if (cmdupd_redir(aux_tkn, (t_token *) aux_lst->content, new_cmd))
				aux_lst = cmdupd_err(aux_lst, &new_cmd);
		}
		else if (aux_tkn->type == WORD)
			cmdupd_argv(aux_tkn, new_cmd);
		else if (aux_tkn->type == PIPE)
			cmdupd_save(aux_lst, &new_cmd, minishell);
		aux_lst = aux_lst->next;
	}
	cmdupd_save(NULL, &new_cmd, minishell);
}
