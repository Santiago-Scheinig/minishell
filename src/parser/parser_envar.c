/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:54:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/14 20:04:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

/**
 * I split the words inside token_lst with ' ', using ft_iq_split to avoid split
 * between spaces, then i make every single word diveded by such into a new token
 * starting in the current position of the list.
 */
static void	envar_tkn(t_list *token_lst, t_body *minishell)
{
	char	**split;
	int i;

	i = 0;
	split = ft_iq_split(((t_token *) token_lst->content)->str, ' ');
	if (!split)
		sigend(minishell, 1);
	while (split[i])
		i++;
	while (i && split[--i])
	{
		if (!split[1])
			break;
		if (addlst_here(token_lst, split[i], i))
		{
			ft_split_free(split);
			sigend(minishell, 1);
		}
	}
	free(split);
}

static int	envar_exp(t_list *token_lst, char *str, int start, t_body *minishell)
{
	char	*env_pathname;
	char	*env_value;
	char	*ret;

	if (str[start + 1] && (!ft_isalnum(str[start + 1]) 
		&& str[start + 1] != '_' && str[start + 1] != '?'))
			return (start + 1);
	env_pathname = envar_pathname(&(str[start + 1]));
	if (!env_pathname)
		sigend(minishell, 1);
	env_value = getenv(env_pathname);
	free(env_pathname);
	if (!env_value)
		ret = exp_value(str, start--, env_value);
	else
	{
		ret = exp_value(str, start, env_value);
		if (!ret)
			sigend(minishell, 1);
		if (((t_token *) token_lst->content)->str)
			free(((t_token *) token_lst->content)->str);
		((t_token *) token_lst->content)->str = ret;
	}
	return(start);
}

static void	envar_syn(t_list *token_lst, t_body *minishell)
{
	t_token	*word;
	int		i;
	int		quote;
	char	*tmp;

	i = -1;
	quote = 0;
	tmp = NULL;
	word = (t_token *) token_lst->content;
	while (word->str[++i])
	{
		if (word->str[i] == '\'')
			quote = '\'';
		if (word->str[i] == '\"')
			quote = '\"';
		if (quote)
			tmp = ft_strchr(&word->str[i + 1], quote);
		if (tmp)
			while (word->str[i] && word->str[++i] != quote)
				while (word->str[i] == '$' && quote != '\'')
					i = envar_exp(token_lst, word->str, i, minishell);
		if (word->str[i])
			while (word->str[i] == '$')
				i = envar_exp(token_lst, word->str, i, minishell);
	}
}

/**
 * Verifies if any WORD token includes a valid '$' OPERATOR. If it does, 
 * it expands it inside the T_TOKEN list following the quoting rules for
 * expansion of enviromental variables:
 * 
 * - Single quote enclosing: The enviromental variable remains as plain text.
 * 
 * - Double quote enclosing: The enviromental variable expands, but remains as
 * part of the WORD it was enclosed into.
 * 
 * - No quote enclosing: The enviromental variable expands and becomes 
 * tokenized into WORDS divided only by ' ' (OPERATORS are treated as plain
 * text after expansion).
 */
void	parser_envar(t_body *minishell)
{
	t_list	*token_lst;
	t_token *content;
	int		i;

	token_lst = minishell->token_lst;
	while(token_lst)
	{
		content = (t_token *) token_lst->content;
		i = -1;
		while (content->str && content->type == WORD && content->str[++i])
		{
			if (content->str[i] == '$')
			{
				if (content->str[i + 1] && (ft_isalnum(content->str[i + 1]) 
				|| content->str[i + 1] == '_' || content->str[i + 1] == '?'))
				{
					envar_syn(token_lst, minishell);
					envar_tkn(token_lst, minishell);
					break;
				}
			}
		}
		token_lst = token_lst->next;
	}
}
