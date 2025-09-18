/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:52:50 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static void	parser_input(t_body *minishell)
{
	char	*tmp;

	if (minishell->interactive)
	{
		tmp = shell_pmtexp(minishell->envp_lst);
		if (!tmp)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		minishell->input = readline(tmp);
		free(tmp);
	}
	else
		minishell->input = get_next_line(STDIN_FILENO);
	if (minishell->input == NULL)
		end_minishell(minishell);
	else if (!minishell->input[0])
		parser_input(minishell);
	else if (minishell->interactive && minishell->input[0] != '\0')
		add_history(minishell->input);
}

/**
 * COMMENT UPDATE
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
int	parser(t_body *minishell)
{
	char	**split;

	cleanup(minishell);
	parser_input(minishell);
	split = shell_split(minishell->input);
	if (!split)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	parser_token(minishell, split);
	if (g_signal_received)
	{
		g_signal_received = 0;
		return (MSHELL_MISSUSE);
	}
	parser_envar(minishell);
	parser_cmds(minishell);
	shell_lstclear(&(minishell->token_lst), shell_lstdeltkn);
	minishell->token_lst = NULL;
	return (MSHELL_SUCCESS);
}
