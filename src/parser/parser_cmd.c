/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/29 19:30:31 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"
#include "parser.h"

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

static t_cmd	*cmd_init(t_list *token_lst, t_body *minishell)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	memset(new_cmd, 0, sizeof(t_cmd));
	new_cmd->outfile = 1;
	new_cmd->heredoc[0] = -1;
	new_cmd->heredoc[1] = -1;
	argc = cmd_argc(token_lst) + 1;
	new_cmd->argv = malloc((argc) * sizeof(char *));
	if (!new_cmd->argv)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	memset(new_cmd->argv, 0, (argc * sizeof(char *)));
	return (new_cmd);
}

/*This needs either get minishell, or returns something, 2 mallocs are being made!*/
static void	cmd_upd(t_cmd *new, t_token *aux)
{
	int		i;

	i = 0;
	while (new->argv[i])
		i++;
	new->argv[i] = shell_substr(aux->str, aux->mask, 0, ft_strlen(aux->str));
	if (aux->str)
		free(aux->str);
	aux->str = NULL;
}

static void	cmd_save(t_body *minishell, t_cmd **aux, t_list *token_lst)
{
	t_token	*pipe;
	t_list	*new_node;

	new_node = ft_lstnew((*aux));
	if (!new_node)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	ft_lstadd_back(&(minishell->cmd_lst), new_node);
	if (token_lst)
	{
		pipe = (t_token *) token_lst->content;
		(*aux) = cmd_init(token_lst, minishell);
	}
}

void	parser_cmds(t_body *minishell)
{
	t_list	*lst_aux;
	t_cmd	*new_cmd;
	t_token	*token_aux;
	t_token	*token_next;

	lst_aux = minishell->token_lst;
	new_cmd = cmd_init(minishell->token_lst, minishell);
	while (lst_aux)
	{
		token_aux = (t_token *) lst_aux->content;
		if (lst_aux->next)
			token_next = (t_token *) lst_aux->next->content;
		if (token_aux->type == REDIR_IN || token_aux->type == REDIR_OUT
			|| token_aux->type == REDIR_APPEND || token_aux->type == HEREDOC)
		{
			cmd_redirupd(token_aux, token_next, new_cmd);
			lst_aux = lst_aux->next;
		}
		else if (token_aux->type == WORD)
			cmd_upd(new_cmd, token_aux);
		else if (token_aux->type == PIPE)
			cmd_save(minishell, &new_cmd, lst_aux->next);
		lst_aux = lst_aux->next;
	}
	cmd_save(minishell, &new_cmd, NULL);
}
