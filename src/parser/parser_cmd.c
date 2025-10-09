/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/08 23:14:02 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
static int	cmd_argc(t_list *lst_t_token)
{
	t_token	*aux;
	int		prev_type;
	int		count;

	count = 0;
	prev_type = WORD;
	while (lst_t_token)
	{
		aux = (t_token *) lst_t_token->content;
		if (aux->type == PIPE)
			break ;
		if (aux->type == WORD && prev_type == WORD)
			count++;
		prev_type = aux->type;
		lst_t_token = lst_t_token->next;
	}
	return (count);
}

/**
 * COMMENT PENDING
 */
static t_cmd	*cmd_init(t_list *lst_t_token)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	memset(new_cmd, 0, sizeof(t_cmd));
	new_cmd->infd = STDIN_FILENO;
	new_cmd->outfd = STDOUT_FILENO;
	argc = cmd_argc(lst_t_token) + 1;
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
	if (!aux->str[0] && !i)
	{
		free(aux->str);
		new_cmd->argv[0] = NULL;
	}
	else
		new_cmd->argv[i] = aux->str;
	aux->str = NULL;
}

/**
 * COMMENT PENDING
 */
static void	cmdupd_save( t_list *lst_t_token, t_cmd **new_cmd, t_body *msh)
{
	t_list	*new_node;

	new_node = ft_lstnew((*new_cmd));
	if (!new_node)
		forcend(msh, "malloc", MSHELL_FAILURE);
	ft_lstadd_back(&(msh->lst_t_cmd), new_node);
	if (lst_t_token)
	{
		(*new_cmd) = cmd_init(lst_t_token->next);
		if (!(*new_cmd))
			forcend(msh, "malloc", MSHELL_FAILURE);
	}
}


void	parser_cmds(t_body *msh)
{
	t_cmd	*new_cmd;
	t_list	*lst_t_token;
	t_token	*aux;

	lst_t_token = msh->lst_t_token;
	new_cmd = cmd_init(msh->lst_t_token);
	if (!new_cmd)
		forcend(msh, "malloc", MSHELL_FAILURE);
	while (lst_t_token)
	{
		aux = (t_token *) lst_t_token->content;
		if (aux->type && aux->type != PIPE && aux->type != END)
		{
			lst_t_token = lst_t_token->next;
			msh->exit_no = cmdupd_redir(aux, (t_token *) lst_t_token->content, new_cmd);
			if (msh->exit_no)
				lst_t_token = cmdupd_err(lst_t_token, &new_cmd);
		}
		else if (aux->type == WORD)
			cmdupd_argv(aux, new_cmd);
		else if (aux->type == PIPE)
			cmdupd_save(lst_t_token, &new_cmd, msh);
		lst_t_token = lst_t_token->next;
	}
	cmdupd_save(NULL, &new_cmd, msh);
}
