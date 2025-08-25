/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:54:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 21:43:01 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * Splits the WORD string and adds the words in sequence after the current
 * position of token_lst, expanding the tokens using 'space' as the divisor
 * operator, which aren't protected by quotes.
 * 
 * @param token_lst A pointer to the current position on the token list.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If the ARRAY of STRINGS is just one word, this step is skipped.
 */
static void	envar_tkn(t_list *token_lst, t_body *minishell)
{
	char	**split;
	int i;

	i = 0;
	split = ft_iq_split(((t_token *) token_lst->content)->str, ' ');
	if (!split)
		sigend(minishell, 1);
	while (split[i])
		i++;
	while (i && split[--i])
	{
		if (!split[1])
		{
			ft_split_free(split);
			split = NULL;
			break;
		}
		if (addlst_here(token_lst, split[i], i))
		{
			ft_split_free(split);
			sigend(minishell, 1);
		}
	}
	if (split)
		free(split);
}

/**
 * Finds the value of the declared enviromental variable if any, then expands 
 * it accordingly.
 * 
 * - If there's a value, the WORD string becomes reallocated and expanded with
 * the new values.
 * 
 * - Otherwise, the WORD string becomes cut from memory, without realocation
 * and ereasing every character of the enviromental variable's name.
 * 
 * @param token_lst A pointer with the current position on the token_lst.
 * @param str A STRING with the original string inside of the WORD token.
 * @param start The index where to start on the WORD string.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
static int	envar_exp(t_token *word, int start, char mask, t_body *minishell)
{
	char	*env_pathname;
	char	*env_value;
	char	*ret;

	env_pathname = envar_pathname(&(str[start + 1]));
	if (!env_pathname)
		sigend(minishell, 1);
	env_value = getenv(env_pathname);//i need to change this to isma function, after i solve all that shit
	free(env_pathname);
	if (!env_value)
		ret = exp_value(str, start--, env_value);
	else
	{
		ret = exp_value(str, start, env_value);
		if (!ret)
			sigend(minishell, 1);
		free(((t_token *) token_lst->content)->str);
		((t_token *) token_lst->content)->str = ret;
	}
	exp_mask;
	return(start);
}

/**
 * Analizes the WORD syntaxis and expands all enviromental variables avalible
 * inside of it following the quoting rules for expansion of enviromental
 * variables.
 * 
 * @param token_lst A pointer with the current position on the token_lst.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
static void	envar_syn(t_list *token_lst, t_body *minishell)
{
	t_token	*word;
	int		i;
	int		quote;
	char	*tmp;

	i = -1;
	quote = 0;
	tmp = NULL;
	word = (t_token *) token_lst->content;
	while (word->str[++i])
	{
		while (word->str[i] == '$' && word->mask[i] != 'S')
		{
			if (word->str[i + 1] && (!ft_isalnum(word->str[i + 1]) 
			&& word->str[i + 1] != '_' && word->str[i + 1] != '?'))
			{
				i++;
				continue;
			}
			i = envar_exp(word, i, word->mask[i], minishell);
		}
	}
}

/**
 * Expands every enviromental value that could be inside of a WORD token 
 * if it does, it expands it inside the T_TOKEN list following the 
 * quoting rules for expansion of enviromental variables:
 * 
 * - Single quote enclosing: The enviromental variable remains as plain text.
 * 
 * - Double quote enclosing: The enviromental variable expands, but remains as
 * part of the WORD it was enclosed into.
 * 
 * - No quote enclosing: The enviromental variable expands and becomes 
 * tokenized into WORDS divided only by ' ' (OPERATORS are treated as plain
 * text after expansion).
 * 
 * @param minishell A pointer to the main enviroment structure of minishell
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
void	parser_envar(t_body *minishell)
{
	t_list	*token_lst;
	t_token *content;

	token_lst = minishell->token_lst;
	while(token_lst)
	{
		content = (t_token *) token_lst->content;
		if (content->str && content->type == WORD)
		{
			envar_syn(token_lst, minishell);
			envar_tkn(token_lst, minishell);
		}
		token_lst = token_lst->next;
	}
}
