/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_enviroment_var_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:58:42 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/12 17:29:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * Creates and allocates a new T_TOKEN node.
 * @param str A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
static t_token	*token_dup(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = WORD;
	return (new);
}

void	*lstadd_exp(t_list **token_lst, char **split)
{
	t_token	*aux;
	t_list	*new_node;
	int i;

	i = -1;
	while (split[++i])
	{
		aux = token_dup(split[i]);
		if (!aux)
			return (NULL);
		if (!i)
		{
			free((*token_lst)->content);
			(*token_lst)->content = aux;
			continue;
		}
		new_node = ft_lstnew(aux);
		if (!new_node)
		{
			free(aux);
			return (NULL);
		}
		new_node->next = (*token_lst)->next;
		(*token_lst)->next = new_node;
		(*token_lst) = new_node;
	}
	return (token_lst);
}

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

int	envar_len(char *env_var)
{
	int	i;

	i = 1;
	while (ft_isalnum(env_var[i]) || env_var[i] == '_')
		i++;
	return (i);
}