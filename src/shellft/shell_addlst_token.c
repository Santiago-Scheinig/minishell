/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_addlst_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:29:47 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * COMMENT PENDING
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
 * COMMENT PENDING
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
static t_token	*token_dup(char *str)
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

static int replace_token(t_token *word, char *str)
{
	if (word->str)
		free(word->str);
	word->str = str;
	memset(word->mask, 0, ft_strlen(word->mask));
	memset(word->mask, 'N', ft_strlen(str));
	return (0);
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
int	shell_addlst_token(t_list *token_list, char *str, int start)
{
	t_token *aux;
	t_list	*new_node;
	
	if (!start)
		return (replace_token(((t_token *) token_list->content), str));
	aux = token_dup(str);
	if (!aux)
		return (1);
	aux->type = WORD;
	memset(aux->mask, 'N', ft_strlen(str));
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
