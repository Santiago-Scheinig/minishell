/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/22 22:31:12 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	token_heredoc(t_token *aux, t_token *next, t_body *minishell)
{
	int	fd[2];

	if (next->type != WORD)
		return (sigend(next->str, MSHELL_MISSUSE, minishell));
	if (pipe(fd) == -1)
		return (sigend(next->str, MSHELL_FAILURE, minishell));
	aux->heredoc = heredoc_dup(next, fd, minishell->envp_lst, minishell->exit_no);
	if (aux->heredoc == -1)
		return (sigend(next->str, MSHELL_FAILURE, minishell));
	return (MSHELL_SUCCESS);
}

/**
 * COMMENT UPDATE
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
static int	token_syntax(t_body *minishell)
{
	t_list	*token_lst;
	t_token	*aux;
	t_token	*next;
	int		i;

	i = 0;
	token_lst = minishell->token_lst;
	while (token_lst->next)
	{
		aux = (t_token *) token_lst->content;
		next = (t_token *) token_lst->next->content;
		if (aux->type == REDIR_IN || aux->type == REDIR_OUT
			|| aux->type == REDIR_APPEND || aux->type == HEREDOC)
			if (token_heredoc(aux, next, minishell))
				return (MSHELL_FAILURE);
		if (aux->type == PIPE && next->type == PIPE)
			return (sigend(next->str, MSHELL_MISSUSE, minishell));
		if (!i && aux->type == PIPE)
			return (sigend(aux->str, MSHELL_MISSUSE, minishell));
		token_lst = token_lst->next;
		i++;
	}
	if (((t_token *) token_lst->content)->type != WORD)
		return (sigend(NULL, MSHELL_MISSUSE, minishell));
	return (0);
}

/**
 * Creates and allocates a new T_TOKEN node.
 * 
 * @param str A pointer to the STRING to be tokenized.
 * @return A pointer to the new T_TOKEN allocation; or NULL in case of error.
 */
t_token	*token_dup(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(t_token));
	new->str = str;
	new->type = get_token_type(str);
	if (new->type == WORD)
	{
		new->mask = mask_dup(str);
		if (!new->mask)
			return (NULL);
	}
	return (new);
}

/**
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
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
			break;
		new_node = ft_lstnew(new_token);
		if (!new_node)
			break;
		ft_lstadd_back(&(minishell->token_lst), new_node);
	}
	if (!new_node || !new_token)
	{
		ft_split_free(&split[i]);
		free(split);
		forcend(minishell, "malloc", MSHELL_FAILURE);
	}
	free(split);
	token_syntax(minishell);
}
