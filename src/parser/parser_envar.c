/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:54:53 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/22 22:47:36 by sscheini         ###   ########.fr       */
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
int	envar_mask(char *str, char *value, char **mask, int start)
{
	char	*ret;
	t_pair 	len;

	if (!mask)
		return (MSHELL_SUCCESS);
	len.var = envar_len(&(str[start]));
	if (!value)
	{
		len.value = 0;
		ret = exp_mask(str, (*mask), start, len);
	}
	else
	{
		len.value = ft_strlen(value);
		ret = exp_mask(str, (*mask), start, len);
		if (!ret)
			return (MSHELL_FAILURE);
		if ((*mask))
			free((*mask));
		(*mask) = ret;
	}
	return(MSHELL_SUCCESS);
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
static int	envar_init(char **str, char **mask, int start, t_list *envp)
{
	char	*env_pathname;
	char	*env_value;
	char	*ret;

	env_pathname = envar_pathname(&((*str)[start + 1]));
	if (!env_pathname)
		return (MSHELL_FAILURE);
	env_value = shell_getenv(envp, env_pathname);
	free(env_pathname);
	if (!env_value)
	{
		if (envar_mask((*str), env_value, mask, start))
			return (MSHELL_FAILURE);
		ret = exp_value((*str), env_value, start);
	}
	else
	{
		ret = exp_value((*str), env_value, start);
		if (!ret || envar_mask((*str), env_value, mask, start))
			return (MSHELL_FAILURE);
		if ((*str))
			free((*str));
		(*str) = ret;
	}
	return(MSHELL_SUCCESS);
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
int	envar_syntax(char **str, char **mask, t_list *envp, int exit_no)
{
	int		i;
	int		quote;

	i = -1;
	quote = 0;
	while ((*str)[++i])
	{
		while ((*str)[i] == '$')
		{
			if (mask && (*mask)[i] == 'S')
				break;
			if (!(*str)[i + 1] || (!ft_isalpha((*str)[i + 1])
				&& (*str)[i + 1] != '_' && (*str)[i + 1] != '?'))
			{
				i++;
				continue;
			}
			if ((*str)[i + 1] == '?')
			{
				if (exp_exitno(str, mask, i, exit_no))
					return (MSHELL_FAILURE);
			}
			else if (envar_init(str, mask, i, envp))
				return (MSHELL_FAILURE);
		}
	}
	return (MSHELL_SUCCESS);
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
	t_list	*envp;
	t_list	*token_lst;
	t_token *tkn;

	envp = minishell->envp_lst;
	token_lst = minishell->token_lst;
	while(token_lst)
	{
		tkn = (t_token *) token_lst->content;
		if (tkn->str && tkn->type == WORD)
		{
			if (envar_syntax(&(tkn->str), &(tkn->mask), envp, minishell->exit_no))
				forcend(minishell, "malloc", MSHELL_FAILURE);
			if (tkn->mask[0] == 'N')
				envar_tokenization(minishell->token_lst, minishell);
		}
		token_lst = token_lst->next;
	}
}
