/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:54:53 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:27:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * Splits the WORD string and adds the words in sequence after the current
 * position of token_lst, expanding the tokens using 'space' as the divisor
 * operator, which aren't protected by quotes.
 * 
 * @param token_lst A pointer to the current position on the token list.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If the ARRAY of STRINGS is just one word, this step is skipped.
 */
static void	envar_tokenization(t_list *token_lst, t_body *minishell)
{
	char	**split;
	int i;

	i = 0;
	split = ft_iq_split(((t_token *) token_lst->content)->str, ' ');
	if (!split)
		forcend(minishell, "malloc", MSHELL_FAILURE);
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
		if (shell_addlst_token(token_lst, split[i], i))
		{
			ft_split_free(split);
			forcend(minishell, "malloc", MSHELL_FAILURE);
		}
	}
	if (split)
		free(split);
}

/**
 * COMMENT UPDATE
 * Updates the WORD string mask to the new value of the declared enviromental 
 * variable if any, and expands it accordingly.
 * 
 * - If there's a value, the WORD token mask becomes reallocated and 
 * expanded with the same mask value.
 * 
 * - Otherwise, the WORD token mask string becomes cut from memory, without 
 * realocation, ereasing every character of the enviromental variable's mask.
 * 
 * @param word A pointer to the T_TOKEN to be expanded.
 * @param value A pointer to the STRING value of the enviromental variable.
 * @param start The position index of the enviromental variable on 
 * the WORD token string.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
static int	envar_mask(t_token *word, char *value, int start, t_body *minishell)
{
	char	*ret;
	int		var_len;
	int		value_len;

	var_len = envar_len(&(word->str[start]));
	if (!value)
		ret = exp_mask(word, start, var_len, 0);
	else
	{
		value_len = ft_strlen(value);
		ret = exp_mask(word, start, var_len, value_len);
		if (!ret)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		if (word->mask)
			free(word->mask);
		word->mask = ret;
	}
	return(start);
}

/**
 * COMMENT UPDATE
 * Finds the value of the declared enviromental variable if any, then expands 
 * it accordingly.
 * 
 * - If there's a value, the WORD token string becomes reallocated and 
 * expanded with the new value.
 * 
 * - Otherwise, the WORD token string becomes cut from memory, without 
 * realocation, ereasing every character of the enviromental variable's name.
 * 
 * @param word A pointer to the T_TOKEN to be expanded.
 * @param start The position index of the enviromental variable on 
 * the WORD token string.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
static int	envar_expansion(t_token *word, int start, t_body *minishell)
{
	char	*env_pathname;
	char	*env_value;
	char	*ret;

	env_pathname = envar_pathname(&(word->str[start + 1]));
	if (!env_pathname)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	env_value = shell_getenv(minishell->envp_lst, env_pathname);
	free(env_pathname);
	if (!env_value)
	{
		envar_mask(word, env_value, start, minishell);
		ret = exp_value(word->str, env_value, start);
	}
	else
	{
		ret = exp_value(word->str, env_value, start);
		if (!ret)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		envar_mask(word, env_value, start, minishell);
		if (word->str)
			free(word->str);
		word->str = ret;
	}
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
static void	envar_syntax(t_list *token_lst, t_body *minishell)
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
			if (!word->str[i + 1] || (!ft_isalpha(word->str[i + 1])
			&& word->str[i + 1] != '_' && word->str[i + 1] != '?'))
			{
				i++;
				continue;
			}
			i = envar_expansion(word, i, minishell);
			if (word->mask[i] == 'N')
				envar_tokenization(token_lst, minishell);
		}
	}
}

/**
 * Loops through all the token list and expands every valid enviromental 
 * variable found in WORD tokens. If an expansion is made, the list expands
 * following the quoting rules for expansion of enviromental variables:
 * 
 * - Single quote enclosing: The enviromental variable remains as plain text.
 * 
 * - Double quote enclosing: The enviromental variable expands, but remains as
 * part of the WORD token it was enclosed into.
 * 
 * - No quote enclosing: The enviromental variable expands and becomes 
 * tokenized into WORD tokens divided only by ' ' (OPERATORS are treated as 
 * plain text after expansion).
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
			envar_syntax(token_lst, minishell);
		token_lst = token_lst->next;
	}
}
