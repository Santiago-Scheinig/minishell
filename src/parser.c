/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/25 19:07:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_token	*tokenize(t_body *minishell, char **split)
{
	t_token	*arr;
	t_token	*new;
	int		i;

	i = -1;
	arr = NULL;
	while(split[++i])
	{
		new = new_token(minishell, split[i]);
		if (!new)
			return (sigend(minishell, 1));
		token_add_back(&arr, new);
	}
	free(split);
	return (arr);
}

void	parser(t_body *minishell, char *input)
{
	t_token	*arr;
	char	**split;

	split = ft_iq_split(input, ' ');
	if (!split)
		sigend(minishell, 1);
	arr = tokenize(minishell, split);
}