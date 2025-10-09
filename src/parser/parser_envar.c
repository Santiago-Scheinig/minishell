/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:54:53 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/05 18:32:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * Splits the WORD string and adds the words in sequence after the current
 * position of lst_t_token, expanding the tokens using 'space' as the divisor
 * operator, which aren't protected by quotes.
 * 
 * @param lst_t_token A pointer to the current position on the token list.
 * @param msh A pointer to the main enviroment structure of msh.
 * @note If the ARRAY of STRINGS is just one word, this step is skipped.
 */
static void	envar_tokenization(t_list *lst_t_token, t_body *msh)
{
	char	**split;
	int		i;

	i = 0;
	split = ft_iq_split(((t_token *) lst_t_token->content)->str, ' ');
	if (!split)
		forcend(msh, "malloc", MSHELL_FAILURE);
	while (split[i])
		i++;
	while (i && split[--i])
	{
		if (!split[1])
		{
			ft_split_free(split);
			split = NULL;
			break ;
		}
		if (shell_addlst_token(split[i], i, lst_t_token))
		{
			ft_split_free(split);
			forcend(msh, "malloc", MSHELL_FAILURE);
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
 * @param msh A pointer to the main enviroment structure of msh.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
int	envar_mask(char *str, char *value, char **mask, int start)
{
	char			*ret;
	t_envar_pair	len;

	if (!mask)
		return (MSHELL_SUCCESS);
	len.var = envar_len(&(str[start]));
	if (!value)
	{
		len.val = 0;
		ret = exp_mask(value, (*mask), start, len);
	}
	else
	{
		len.val = ft_strlen(str);
		ret = exp_mask(value, (*mask), start, len);
		if (!ret)
			return (MSHELL_FAILURE);
		if ((*mask))
			free((*mask));
		(*mask) = ret;
	}
	return (MSHELL_SUCCESS);
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
 * @param msh A pointer to the main enviroment structure of msh.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
static int	envar_init(char **str, char **mask, int start, t_list *lst_t_var)
{
	char	*var_pathname;
	char	*var_value;
	char	*ret;

	var_pathname = envar_pathname(&((*str)[start + 1]));
	if (!var_pathname)
		return (MSHELL_FAILURE);
	var_value = shell_getenv(lst_t_var, var_pathname);
	free(var_pathname);
	if (!var_value)
	{
		if (envar_mask((*str), var_value, mask, start))
			return (MSHELL_FAILURE);
		ret = exp_value((*str), var_value, start);
	}
	else
	{
		ret = exp_value((*str), var_value, start);
		if (!ret || envar_mask((*str), var_value, mask, start))
			return (MSHELL_FAILURE);
		if ((*str))
			free((*str));
		(*str) = ret;
	}
	return (MSHELL_SUCCESS);
}

/**
 * Analizes the WORD syntaxis and expands all enviromental variables avalible
 * inside of it following the quoting rules for expansion of enviromental
 * variables.
 * 
 * @param lst_t_token A pointer with the current position on the lst_t_token.
 * @param msh A pointer to the main enviroment structure of msh.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
int	envar_syntax(char **str, char **mask, t_list *lst_t_var, int exit_no)
{
	int		i;
	int		quote;

	i = -1;
	quote = 0;
	while ((*str) && (*str)[++i])
	{
		while ((*str)[i] == '$')
		{
			if (mask && (*mask)[i] == 'S')
				break ;
			if (!(*str)[i + 1] || (!ft_isalpha((*str)[i + 1])
				&& (*str)[i + 1] != '_' && (*str)[i + 1] != '?'))
				break ;
			if ((*str)[i + 1] == '?')
			{
				if (exp_exitno(str, mask, i, exit_no))
					return (MSHELL_FAILURE);
			}
			else if (envar_init(str, mask, i, lst_t_var))
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
 * @param msh A pointer to the main enviroment structure of msh
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
void	parser_envar(t_body *msh)
{
	t_list	*lst_t_var;
	t_list	*lst_t_token;
	t_token	*aux;

	lst_t_var = msh->lst_t_var;
	lst_t_token = msh->lst_t_token;
	while (lst_t_token)
	{
		aux = (t_token *) lst_t_token->content;
		if (aux->str && aux->type == WORD)
		{
			if (envar_syntax(&aux->str, &aux->mask, lst_t_var, msh->exit_no))
				forcend(msh, "malloc", MSHELL_FAILURE);
			if (aux->mask[0] == 'N')
				envar_tokenization(msh->lst_t_token, msh);
		}
		lst_t_token = lst_t_token->next;
	}
}
