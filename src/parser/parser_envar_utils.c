/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:58:42 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 22:26:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * Calculates the length of the enviroment variable name.
 * 
 * @param env_var A pointer to where the enviromental variable name start's.
 * @return The lenght of the enviroment variable name.
 * @note The minimum lenght size of a enviroment variable is always one,
 * on behalf of the '$' sign.
 */
static int	envar_len(char *env_var)
{
	int	i;

	i = 1;
	while (ft_isalnum(env_var[i]) || env_var[i] == '_')
		i++;
	return (i);
}

char	*exp_mask(t_token *word, int start, char *value)
{
	int 	aux_len;
	int		aux_start;
	char	*aux_mask;
	char	*new_mask;

	if (!value)
	{
		aux_len = envar_len(&(word->str[start]));
		aux_start = start + aux_len;
		aux_len = ft_strlen(&(word->str[aux_start])) + 1;
		memmove(&(word->mask[start]), &(word->mask[aux_start]), aux_len);
		return (word->mask);
	}
	aux_len = ft_strlen(word->str);
	new_mask = ft_calloc(aux_len + 1, sizeof(char));
	if (!new_mask)
		return (NULL);
	ft_strlcpy(new_mask, word->mask, aux_len + 1);
	memset(&new_mask[start], word->mask[start], ft_strlen(value));
	aux_mask = &(word->mask[start + envar_len(&(word->str[start]))]);
	aux_start = ft_strlen(new_mask);
	ft_strlcpy(&new_mask[aux_start], aux_mask, aux_len + 1);
	return (new_mask);
}

/**
 * Expands an enviromental variable and returs the modified value.
 * 
 * @param str A pointer to the WORD string where to expand.
 * @param start An index to the start position of the enviromental
 * variable on [str].
 * @param value A pointer to the STRING to be expanded.
 * @return A pointer to the expanded STRING.
 * @note If value is NULL, no allocation is made and the original
 * [str] becomes cut removing the enviromental variable name of it.
 * Otherwise, it reallocates the new expanded string and returns
 * it.
 */
char	*exp_value(char *str, int start, char *value)
{
	int 	aux_len;
	int		aux_start;
	char	*exp_str;
	char	*aux_str;

	if (!value)
	{
		aux_len = envar_len(&(str[start]));
		aux_start = start + aux_len;
		aux_len = ft_strlen(&(str[aux_start])) + 1;
		memmove(&(str[start]), &(str[aux_start]), aux_len);
		return (str);
	}
	aux_len = ft_strlen(value) + ft_strlen(str) + 1;
	exp_str = ft_calloc(aux_len, sizeof(char));
	if (!exp_str)
		return (NULL);
	ft_strlcpy(exp_str, str, aux_len + 1);
	ft_strlcpy(&exp_str[start], value, aux_len + 1);
	aux_str = &(str[start + envar_len(&(str[start]))]);
	aux_start = ft_strlen(exp_str);
	ft_strlcpy(&exp_str[aux_start], aux_str, aux_len);
	return (exp_str);
}

/**
 * Allocates and returns a clean STRING with only the enviromental variable
 * name, to search into getenv().
 * 
 * @param env_var A pointer to the WORD string on the position where the
 * enviromental variable name starts (Position after the '$').
 * @return A pointer to a new allocated STRING that only includes the name
 * of the enviromental variable.
 */
char	*envar_pathname(char *env_var)
{
	char	*new_path;
	int		env_var_len;
	int		i;

	i = -1;
	env_var_len = envar_len(env_var);
	new_path = ft_calloc((env_var_len + 1), sizeof(char));
	if (!new_path)
		return (NULL);
	while(++i < env_var_len)
		new_path[i] = env_var[i];
	return (new_path);
}

/**
 * Adds a new list node, after the current one, which also includes a 
 * new T_TOKEN content default setted as WORD.
 * 
 * @param token_list A pointer to the current position on the token list.
 * @param str A pointer to the new STRING to use content on the new token.
 * @param start A flag that if equals zero, will replace the content of 
 * the current token node, instead of creating a new node.
 * @return Zero on success. or Malloc error. NEED TO CHANGE THIS LINE!!!!
 */
int	addlst_here(t_list *token_list, char *str, int start)
{
	t_token *aux;
	t_list	*new_node;
	
	if (!start)
	{
		if (((t_token *) token_list->content)->str)
			free (((t_token *) token_list->content)->str);
		((t_token *) token_list->content)->str = str;
		return (0);
	}
	aux = token_dup(str);
	if (!aux)
		return (1);
	aux->type = WORD;
	new_node = ft_lstnew(aux);
	if (!new_node)
	{
		free(aux);
		return (1);
	}
	new_node->next = token_list->next;
	token_list->next = new_node;
	return (0);
}
