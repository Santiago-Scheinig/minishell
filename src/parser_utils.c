/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:41:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/25 19:05:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	get_token_type(char *str)
{
	if (!str)
		return (END);
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 3))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 3))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 2))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 2))
		return (REDIR_OUT);
	return (WORD);
}

/**
 * Searches for a pointer to the last LIST node to be found.
 * @param lst The starting node.
 * @return A pointer to the last LIST node.
 * @note If the starting node doesn't exists, returns NULL.
 */
t_token	*last_token(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

/**
 * Creates and allocates a new LIST node.
 * @param content A pointer to the content to be included inside of the new
 * node.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*new_token(t_body *minishell, char	*content)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (sigend(minishell, 1));
	new->str = content;
	new->type = get_token_type(content);
	new->next = NULL;
	return (new);
}

/**
 * Adds a lst node at the end of a lst HEAD.
 * @param lst The LIST HEAD where to add the new node.
 * @param new The new LIST node to add.
 * @note If the LIST HEAD is empty, sets the new LIST node at the start
 * of it.
 */
void	add_back_token(t_token **lst, t_token *new)
{
	t_token	*tmp;

	tmp = last_token(*lst);
	if (!tmp)
	{
		*lst = new;
		return ;
	}
	tmp->next = new;
}
