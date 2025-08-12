/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_enviroment_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:54:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/12 17:43:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void *empty_exp(t_token *word, int start)
{
	int	aux_len;
	int aux_start;

	aux_len = envar_len(&(word->str[start]));
	aux_start = start + aux_len;
	aux_len = ft_strlen(&(word->str[aux_start])) + 1;
	memmove(&(word->str[start]), &(word->str[aux_start]), aux_len);
	return (word);
}

static void	*token_exp(t_list *token_lst, t_token *word, char *exp_str, int q)
{
	char	**split;

	if(q)
	{
		word->str = exp_str;
		return (word);
	}
	split = ft_split(exp_str, ' ');
	if (!split)
		return (NULL);
	if (!lstadd_exp(&token_lst, split))
	{
		free(split);
		return (NULL);
	}
	return (word);
}

static void	*envar_exp(t_list *token_lst, int start, int q, char *value)
{
	t_token *word;
	char	*exp_str;
	char	*aux_str;
	int		aux_len;
	int		aux_start;

	word = (t_token *) token_lst->content;
	if (!value)
		return (empty_exp(word, start));
	aux_len = ft_strlen(value) + ft_strlen(word->str) + 1;
	exp_str = ft_calloc(aux_len, sizeof(char));
	if (!exp_str)
		return (NULL);
	ft_strlcpy(exp_str, word->str, aux_len + 1);
	ft_strlcpy(&exp_str[start], value, aux_len + 1);
	aux_str = &(word->str[start + envar_len(&(word->str[start]))]);
	aux_start = ft_strlen(exp_str);
	ft_strlcpy(&exp_str[aux_start], aux_str, aux_len);
	word = token_exp(token_lst, word, exp_str, q);
	if (!word)
	{
		free(exp_str);
		return (NULL);
	}
	return (word);
}

static void	envar_dup(t_list *token_lst, int start, int q, t_body *minishell)
{
	t_token	*word;
	char	*env_pathname;
	char	*env_value;
	int		n;

	n = -1;
	word = (t_token *) token_lst->content;
	env_pathname = envar_pathname(&(word->str[start + 1]));
	if (!env_pathname)
		sigend(minishell, 1);
	env_value = getenv(env_pathname);
	free(env_pathname);
	if (!envar_exp(token_lst, start, q, env_value))
		sigend(minishell, 1);
}

void	get_envar(t_list *token_lst, t_body *minishell)
{
	int		i;
	int		quote;
	char	*tmp;
	t_token *word;

	i = -1;
	tmp = NULL;
	word = (t_token *) token_lst->content;
	while (word->str[++i])
	{
		if (word->str[i] == '\'')
			quote = '\'';
		if (word->str[i] == '\"')
			quote = '\"';
		tmp = ft_strchr(&word->str[i + 1], quote);
		if (tmp)
			while (word->str[++i] != quote)
				while (word->str[i] == '$' && quote != '\'')
					envar_dup(token_lst, i, quote, minishell);
		while (word->str[i] == '$')
			envar_dup(token_lst, i, 0, minishell);
	}
}
