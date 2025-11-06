/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 13:54:53 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 15:18:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * @brief	Splits a token into multiple tokens after variable expansion.
 *
 *			When an environment variable expansion produces multiple
 *			space-separated words, this function splits them into new
 *			tokens and inserts them into the token list.
 *
 *			Uses ft_split_iq() to respect quoted substrings during the
 *			split process. If memory allocation fails, forces a shell
 *			exit with an error.
 *
 * @param	lst_token	List node containing the original token.
 * @param	msh			Pointer to main shell structure (t_body).
 *
 * @note	Frees split array after processing. Updates token list.
 */
static void	envar_tokenization(t_list *lst_token, t_body *msh)
{
	char	**split;
	int		i;

	i = 0;
	split = ft_split_iq(((t_token *) lst_token->content)->str, ' ');
	if (!split)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
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
		if (shell_lstadd_newtkn(i, split[i], OPERATORS, lst_token))
		{
			ft_split_free(split);
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		}
	}
	if (split)
		free(split);
}

/**
 * @brief	Updates the mask string after environment variable expansion.
 *
 *			Adjusts the mask for a string segment expanded from an
 *			environment variable, ensuring character classification
 *			remains consistent (e.g., quoted, normal, expanded).
 *
 *			Calls exp_mask() to rebuild the mask substring around the
 *			expansion site.
 *
 * @param	start	Index in str where expansion begins.
 * @param	str		Original string containing the variable.
 * @param	value	Value string of the variable (may be NULL).
 * @param	mask	Pointer to mask string associated with str.
 *
 * @note	If mask is NULL, the function does nothing.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int	envar_mask(int start, char *str, char *value, char **mask)
{
	char			*tmp;
	t_envar_pair	len;

	if (!mask)
		return (MSHELL_SUCCESS);
	len.var = envar_len(&(str[start]));
	if (!value)
	{
		len.val = 0;
		tmp = exp_mask(start, value, (*mask), len);
	}
	else
	{
		len.val = ft_strlen(str);
		tmp = exp_mask(start, value, (*mask), len);
		if (!tmp)
			return (MSHELL_FAILURE);
		if ((*mask))
			free((*mask));
		(*mask) = tmp;
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Expands a single environment variable reference in a string.
 *
 *			Extracts the variable name after a '$' at index 'start',
 *			retrieves its value from the environment list, and replaces
 *			the reference with the corresponding value. Updates both
 *			the string and its mask to reflect the change.
 *
 *			Static helpers:
 *
 *				- envar_mask():		Rebuilds the mask after expansion.
 *
 * @param	start		Position in str where '$' occurs.
 * @param	str			Pointer to string to expand.
 * @param	mask		Pointer to mask string (may be NULL).
 * @param	head_envar	List of environment variables.
 *
 * @note	If the variable is undefined, replaces it with an empty
 *			string and adjusts the mask accordingly.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
static int	envar_init(int start, char **str, char **mask, t_list *head_envar)
{
	char	*envar_path;
	char	*envar_value;
	char	*tmp;

	envar_path = envar_pathname(&((*str)[start + 1]));
	if (!envar_path)
		return (MSHELL_FAILURE);
	envar_value = shell_envchr(NULL, envar_path, head_envar);
	free(envar_path);
	if (!envar_value)
	{
		if (envar_mask(start, (*str), envar_value, mask))
			return (MSHELL_FAILURE);
		tmp = exp_value(start, (*str), envar_value);
	}
	else
	{
		tmp = exp_value(start, (*str), envar_value);
		if (!tmp || envar_mask(start, (*str), envar_value, mask))
			return (MSHELL_FAILURE);
		if ((*str))
			free((*str));
		(*str) = tmp;
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Parses and expands environment variables in a string.
 *
 *			Iterates through 'str', replacing every valid '$VAR' or
 *			'$?' pattern with its corresponding value. Ignores '$'
 *			inside single-quoted segments marked in 'mask'.
 *
 *			Calls envar_init() for named variables and exp_exitno()
 *			for special '$?' cases. The mask is updated accordingly.
 *
 *			Static helpers:
 *
 *				- envar_init():		Expands named variables.
 *				- exp_exitno():		Handles '$?' exit code expansion.
 *
 * @param	exit_no	Last command's exit status (for $? replacement).
 * @param	str		Pointer to string to expand.
 * @param	mask	Optional mask string marking quoted regions.
 * @param	head_envar	Linked list of environment variables.
 *
 * @note	Expands variables only outside single quotes.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int	envar_syntax(int exit_no, char **str, char **mask, t_list *envar)
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
				if (exp_exitno(i, exit_no, str, mask))
					return (MSHELL_FAILURE);
			}
			else if (envar_init(i, str, mask, envar))
				return (MSHELL_FAILURE);
		}
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Expands environment variables in all word tokens.
 *
 *			Traverses the token list, expanding variables within each
 *			token string using envar_syntax(). Tokens marked with 'N'
 *			in their mask after expansion are split again via
 *			envar_tokenization() to handle multi-word expansions.
 *
 *			Static helpers:
 *
 *				- envar_syntax():	Performs actual string expansion.
 *				- envar_tokenization():	Splits expanded tokens.
 *
 * @param	msh	Pointer to main shell structure (t_body).
 *
 * @note	Modifies token list in-place. May allocate new tokens.
 */
void	parser_envar(t_body *msh)
{
	t_list	*head_envar;
	t_list	*lst_token;
	t_token	*aux;

	head_envar = msh->head_envar;
	lst_token = msh->head_token;
	while (lst_token)
	{
		aux = (t_token *) lst_token->content;
		if (aux->str && aux->type == WORD)
		{
			if (envar_syntax(msh->exit_no, &aux->str, &aux->mask, head_envar))
				shell_forcend(MSHELL_FAILURE, "malloc", msh);
			if (aux->mask[0] == 'N')
				envar_tokenization(msh->head_token, msh);
		}
		lst_token = lst_token->next;
	}
}
