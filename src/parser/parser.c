/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/27 00:57:00 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

void	sigend(t_body *minishell, int errno)
{
	if (minishell || errno)
	{
		if (errno == 3)
		{
			printf("Invalid input\n");
			kill(/** PID probably 0*/,SIGUSR1);	
		}
		return ;
	}
	return;	
}


/**
 * Analizes user input, validates it's syntax and saves a list of commands
 * to execute on the minishell enviroment structure. The parsing is diveded
 * in 4 different steps.
 * 
 * - READ_INPUT: Reads user input from user stopping at a newline, and 
 * intercepts user signals.
 * - TOKENIZATION: Breaks the input line into tokens (words, operators).
 * - VARIABLE_EXPANSION: Replaces and expands all $VAR on every token.
 * - QUOTE_REMOVAL: Removes quotes '\' and ';' since they aren't interpreted.
 * 
 * @param minishell A pointer to the minishell enviroment structure.
 * @note If any error occurs during the parsing, the function will end with
 * a sigend([errno]) call.
 */
void	parser(t_body *minishell)
{
	char	**split;

	parser_input(minishell);
	split = shell_split(minishell->input);
	if (!split)
		sigend(minishell, 1);
	parser_token(minishell, split);
	parser_envar(minishell);
	parser_cmds(minishell);
	shell_lstclear(&(minishell->token_lst), shell_lstdeltkn);
	minishell->token_lst = NULL;
}
