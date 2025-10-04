/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lstclear.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:32:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 20:50:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Deletes an environment variable node and its associated memory.
 *
 * Frees the memory used by a t_var stored in list->content, including
 * the name and value strings. Also frees the node itself.
 *
 * @param list	Pointer to the environment variable list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes list->content points to a valid t_var.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_var(t_list *lst, void (*del)(void *))
{
	t_list	*tmp;
	t_var	*aux;

	tmp = lst;
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
 * @brief Deletes a command node and its associated memory.
 *
 * Frees the memory used by a t_cmd stored in lst->content, including
 * the pathname string and the argv array with all its elements.
 * Also frees the node itself.
 *
 * @param lst	Pointer to the command list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes lst->content points to a valid t_cmd.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_cmd(t_list *lst, void (*del)(void *))
{
	t_cmd	*cmd;
	int		i;

	if (lst->content)
	{
		cmd = (t_cmd *) lst->content;
		if (cmd->argv)
		{
			i = -1;
			while (cmd->argv[++i])
				del(cmd->argv[i]);
			del(cmd->argv);
		}
		if (cmd->pathname)
			del(cmd->pathname);
		cmd->argv = NULL;
		cmd->pathname = NULL;
		del(lst->content);
	}
	del(lst);
}

/**
 * @brief Deletes a token node and its associated memory.
 *
 * Frees the memory used by a t_token stored in lst->content, including
 * the string and mask. Also frees the node itself.
 *
 * @param lst	Pointer to the token list node to delete.
 * @param del	Function pointer to free allocated memory.
 *
 * @note	This function assumes lst->content points to a valid t_token.
 * @note	After calling this function, the node and its contents are
 *			completely freed; do not access them afterwards.
 */
void	shell_lstdel_tkn(t_list *lst, void (*del)(void *))
{
	t_token	*word;

	if (lst->content)
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
 * @brief Clears and frees a linked list.
 *
 * Iterates through the list pointed to by lst, applying the provided
 * del function to each node and its content. Frees each node and
 * sets the list pointer to NULL at the end.
 *
 * @param lst	Pointer to the pointer of the linked list to clear.
 * @param del	Function pointer to a function that deletes a node's content.
 *
 * @note	The del function must handle freeing any allocated memory
 *			in the node content. The list structure itself is freed
 *			by this function.
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
