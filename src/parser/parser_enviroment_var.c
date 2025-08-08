/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_enviroment_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:54:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/08 18:36:40 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static void *empty_exp(t_list *token_lst, t_token *word, int start, char *envp)
{
	int	aux_len;
	int aux_start;

	aux_len = envar_len(&(word->str[start]));
	aux_start = start + aux_len;
	aux_len = ft_strlen(&(word->str[aux_start])) + 1;
	memmove(&(word->str[start], &(word->str[aux_start]), aux_len));
	return (token_lst);
}

static void	*token_exp(t_list *token_lst, int start, int q, char *envp)
{
	if(q)
	{
		
	}
}

static void	*envar_exp(t_list *token_lst, int start, int q, char *envp)
{
	t_token *word;
	char	*aux_str;
	char	*exp_str;
	int		aux_len;
	int		aux_start;

	word = (t_token *) token_lst->content;
	if (!envp)
		return (empty_exp(token_lst, word, start, envp));
	aux_str = envar_pathval(envp);
	if (!aux_str)
		return (NULL);
	aux_len = ft_strlen(aux_str) + ft_strlen(word->str) + 1;
	exp_str = ft_calloc(aux_len, sizeof(char));
	if (!exp_str)
		return (NULL);
	ft_strlcpy(exp_str, word->str, aux_len + 1);
	ft_strlcpy(exp_str[start], aux_str, aux_len + 1);
	free(aux_str);
	aux_str = &(word->str[start + envar_len(&(word->str[start]))]);
	aux_start = ft_strlen(exp_str);
	ft_strlcpy(exp_str[aux_start], aux_str, aux_len);
	return (token_exp(token_lst, exp_str, q));
}

static void	envar_dup(t_list *token_lst, int start, int q, t_body *minishell)
{
	t_token	*word;
	char	*env_path;
	int		n;

	n = -1;
	word = (t_token *) token_lst->content;
	env_path = envar_pathsrc(&(word->str[start + 1]));
	if (!env_path)
		sigend(minishell, 1);
	while (envp[++n])
	{
		if (!ft_strncmp(envp[n], env_path, (envlen(env_path) + 1)))
		{
			if (!envar_exp(token_lst, start, q, envp[n]))
				sigend(minishell, 1);
			break;
		}
	}
	if (!envp[n])
		if (!envar_exp(token_lst, start, q, NULL))
			sigend(minishell, 1);
	free(env_path);
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
		tmp = ft_strchr(&word.str[i + 1], quote);
		if (tmp)
			while (word->str[++i] != quote)
				while (word->str[i] == '$' && quote != '\'')
					envar_dup(token_lst, i, quote, minishell);
		tmp = NULL;
		while (word->str[i] == '$')
			envar_dup(token_lst, i, 0, minishell);
	}
}
