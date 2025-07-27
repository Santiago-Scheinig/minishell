/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/27 16:41:11 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"


static int	verify_tokens(t_body *minishell, t_token *arr)
{
	
}

static t_token	*tokenize(t_body *minishell, char **split)
{
	t_list	**token_lst;
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	tokens = NULL;
	while(split[++i])
	{
		new_token = create_token(minishell, split[i]);
		new = ft_lstnew(new_token);
		if (!new)
			sigend(minishell, 1);
		add_back_token(&arr, new);
	}
	free(split);
	return (arr);
}

t_cmd	*parser(t_body *minishell, char *input)
{
	t_token	*arr;
	char	**split;

	split = ft_iq_split(input, ' ');
	if (!split)
		sigend(minishell, 1);
	free(input);
	arr = tokenize(minishell, split);
	if (!arr)
		sigend(minishell, 2);
	if (verify_tokens(minishell, arr))
		sigend(minishell, 2);
}