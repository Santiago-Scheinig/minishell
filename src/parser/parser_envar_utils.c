/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:58:42 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/14 20:05:05 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static int	envar_len(char *env_var)
{
	int	i;

	i = 1;
	while (ft_isalnum(env_var[i]) || env_var[i] == '_')
		i++;
	return (i);
}

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