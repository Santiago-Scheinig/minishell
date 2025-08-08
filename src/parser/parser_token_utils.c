/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/08 19:00:18 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * Verifies if any WORD token includes a valid '$' OPERATOR. If it does, 
 * it expands it inside the T_TOKEN list following the quoting rules for
 * expansion of enviromental variables:
 * 
 * - Single quote enclosing: The enviromental variable remains as plain text.
 * 
 * - Double quote enclosing: The enviromental variable expands, but remains as
 * part of the WORD it was enclosed into.
 * 
 * - No quote enclosing: The enviromental variable expands and becomes 
 * tokenized into WORDS divided only by ' ' (OPERATORS are treated as plain
 * text after expansion).
 */
static void	verify_envar(t_body *minishell)
{
	t_list	*token_lst;
	t_token *content;
	int		i;

	token_lst = minishell->token_lst;
	while(token_lst)
	{
		content = (t_token *) token_lst->content;
		if (content->type == WORD)
		{
			i = -1;
			if (content->str[++i] == '$')
			{
				if (!content->str[i + 1] || !ft_isalnum(content->str[i + 1]) 
				|| content->str[i + 1] != '_' || content->str[i + 1] != '?')
				{
					token_lst = token_lst->next;
					continue;
				}
				else
					get_envar(token_lst, minishell);
			}
		}
	}
}

/**
 * Verifies the correct sintax of the user input inside a T_TOKEN list
 * by following the bash-shell rules:
 * 
 * - The list cannot beggin with a '|' OPERATOR.
 * 
 * - Any redirection OPERATOR must be followed by it's linked WORD.
 * 
 * - There cannot be two consecutive '|' OPERATORS.
 * 
 * - The list must allways end with a WORD.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 */
static void	verify_parser(t_body *minishell)
{
	t_list	*lst_aux;
	t_token	*token_aux;
	t_token	*token_next;
	int		i;

	i = 0;
	lst_aux = minishell->token_lst;
	while (lst_aux->next)
	{
		token_aux = (t_token *) lst_aux->content;
		if (lst_aux->next)
			token_next = (t_token *) lst_aux->next->content;
		if (token_aux->type == REDIR_IN || token_aux->type == REDIR_OUT
			|| token_aux->type == REDIR_APPEND || token_aux->type == HEREDOC)
			if (token_next->type != WORD)
				sigend(minishell, 3);
		if (token_aux->type == PIPE && token_next->type == PIPE)
			sigend(minishell, 3);
		if (!i && token_aux->type == PIPE)
			sigend(minishell, 3);
		lst_aux = lst_aux->next;
		i++;
	}
	if (((t_token *) lst_aux->content)->type != WORD)
		sigend(minishell, 3);
}

/**
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
static t_token	*token_dup(t_body *minishell, char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		sigend(minishell, 1);
	new->str = str;
	new->type = get_token_type(str);
	return (new);
}

void	tokenize(t_body *minishell, char **split)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	minishell->token_lst = NULL;
	while(split[++i])
	{
		new_token = token_dup(minishell, split[i]);
		new_node = ft_lstnew(new_token);
		if (!new_node)
			sigend(minishell, 1);
		ft_lstadd_back(&(minishell->token_lst), new_node);
	}
	free(split);
	verify_parser(minishell);
	verify_envar(minishell);
}
