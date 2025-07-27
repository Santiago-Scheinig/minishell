/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:41:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/27 16:39:34 by ischeini         ###   ########.fr       */
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
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*create_token(t_body *minishell, char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		sigend(minishell, 1);
	new->str = str;
	new->type = get_token_type(str);
	return (new);
}
