/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/12 16:34:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	sigend(t_body *minishell, int errno)
{
	if (minishell || errno)
	{
		if (errno == 3)
			printf("Invalid input\n");
		return ;
	}
	return;	
}

/**
 * change the split to a specific shell iq_token_split (which ignores whatever
 * is inside quotes (double and single), as well as unclosed quotes)
 */
void	parser(t_body *minishell)
{
	char	**split;

	split = shell_split(minishell->input);
	if (!split)
		sigend(minishell, 1);
	tokenize(minishell, split);
	return ;
	//i should verify this work before keep going.
	get_cmds(minishell);
	ft_lstclear(&(minishell->token_lst), free);
	minishell->token_lst = NULL;
}
