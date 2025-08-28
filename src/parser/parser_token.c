/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/28 19:23:34 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"
#include "parser.h"

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
static int	verify_syntax(t_body *minishell)
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
				return (sigend(MSHELL_MISSUSE));//This signals SIGUSR, which sets global and after parsing tells the loop to restart.
		if (token_aux->type == PIPE && token_next->type == PIPE)
			return (sigend(MSHELL_MISSUSE));//This signals SIGUSR, which sets global and after parsing tells the loop to restart.
		if (!i && token_aux->type == PIPE)
			return (sigend(MSHELL_MISSUSE));//This signals SIGUSR, which sets global and after parsing tells the loop to restart.
		lst_aux = lst_aux->next;
		i++;
	}
	if (((t_token *) lst_aux->content)->type != WORD)
		return (sigend(MSHELL_MISSUSE));//This signals SIGUSR, which sets global and after parsing tells the loop to restart.
}

/**
 * Working like a charm, need to comment only.
 */
static int	maskstr_quoted(char *str, char *mask_str, char quote)
{
	int	i;

	i = 0;
	mask_str[i] = 'O';
	while (str[++i] && str[i] != quote)
	{
		if (quote == '\'')
			mask_str[i] = 'S';
		else if (quote == '\"')
			mask_str[i] = 'D';
	}
	mask_str[i] = 'O';
	return (i);
}

/**
 * Working like a charm, need to comment only.
 */
static char	*maskstr(char *str)
{
	int		i;
	char	quote;
	char	*mask_str;
	
	mask_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!mask_str)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		quote = 0;
		if (str[i] == '\"' && str[i + 1] && ft_strchr(&str[i + 1], '\"'))
			quote = '\"';
		else if (str[i] == '\'' && str[i + 1] && ft_strchr(&str[i + 1], '\''))
			quote = '\'';
		if (quote)
			i += maskstr_quoted(&str[i], &mask_str[i], quote);
		else if (str[i] == ';' || str[i] == '\\'
			|| str[i] == '\'' || str[i] == '\"')
			mask_str[i] = 'O';
		else
			mask_str[i] = 'N';
	}
	return (mask_str);
}

/**
 * Creates and allocates a new T_TOKEN node.
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
		new->mask = maskstr(str);
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
	verify_syntax(minishell);
}
