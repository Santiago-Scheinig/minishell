/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstclear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:32:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/16 19:28:30 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "shellft.h"

void	shell_lstdelvar(t_list *list, void (*del)(void *))
{
	t_list	*tmp;
	t_var	*aux;

	tmp = list;
	aux = (t_var *)tmp->content;
	if (aux)
	{
		if (aux->name)
			del(aux->name);
		if (aux->value)
			del(aux->value);
		del(aux);
	}
	del(tmp);
}

/**
 * Frees the content of a COMAND NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void	shell_lstdelcmd(t_list *lst, void (*del)(void *))
{
	t_cmd	*cmd;
	int		i;

	if (lst->content != NULL)
	{
		cmd = (t_cmd *) lst->content;
		if (cmd->argv)
		{
			i = -1;
			while (cmd->argv[++i])
				del(cmd->argv[i]);
			del(cmd->argv);
		}
		if (cmd->limitator)
			del(cmd->limitator);
		if (cmd->pathname)
			del(cmd->pathname);
		if (cmd->infile > 0)
			close(cmd->infile);
		if (cmd->outfile > 1)
			close(cmd->outfile);
		if (cmd->heredoc[0] > 0)
			close(cmd->heredoc[0]);
		if (cmd->heredoc[1] > 0)
			close(cmd->heredoc[1]);
		cmd->argv = NULL;
		cmd->limitator = NULL;
		cmd->pathname = NULL;
		del(lst->content);
	}
	del(lst);
}

/**
 * Frees the content of a TOKEN NODE. then frees the NODE.
 * @param lst The LIST node to free.
 * @param del The function used to free the content.
 * @note In general, the del function should be free().
 */
void	shell_lstdeltkn(t_list *lst, void (*del)(void *))
{
	t_token	*word;

	if (lst->content != NULL)
	{
		word = (t_token *) lst->content;
		if (word->str)
			del(word->str);
		if (word->mask)
			del(word->mask);
		word->str = NULL;
		word->mask = NULL;
		del(lst->content);
	}
	del(lst);
}

/**
 * Frees every node included on the LIST HEAD. 
 * @param lst The LIST HEAD to free.
 * @param del The function used to free each node.
 * @note In general, the del function should be free().
 */
void	shell_lstclear(t_list **lst, void (*del)(t_list *, void (*)(void *)))
{
	t_list	*tmp;

	while ((*lst))
	{
		tmp = (*lst)->next;
		del((*lst), free);
		(*lst) = tmp;
	}
}
