/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/14 19:28:03 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*token_dup(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = get_token_type(str);
	return (new);
}

/**
 * Verifies the correct syntax of the user input inside a T_TOKEN list
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
static void	verify_syntax(t_body *minishell)
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
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call,
 */
void	parser_token(t_body *minishell, char **split)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	minishell->token_lst = NULL;
	while(split[++i])
	{
		new_token = token_dup(split[i]);
		if (!new_token)
			sigend(minishell, 1);
		new_node = ft_lstnew(new_token);
		if (!new_node)
			sigend(minishell, 1);
		ft_lstadd_back(&(minishell->token_lst), new_node);
	}
	free(split);
	verify_syntax(minishell);
}
