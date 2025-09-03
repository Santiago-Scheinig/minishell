/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_addlst_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 15:29:19 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/03 21:56:36 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

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
