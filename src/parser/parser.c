/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/21 19:07:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_cmd.h"

/**
 * COMMENT PENDING
 */
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
		minishell->input = get_next_line(STDIN_FILENO);//if gnl fails via malloc or read, it must terminate the shell!
	if (minishell->input == NULL)
		msh_exit(NULL, minishell);
	else if (!minishell->input[0])
	{
		free(minishell->input);
		parser_input(minishell);
	}
}

/**
 * Analizes user input, validates it's syntax and saves a list of commands
 * to execute on the minishell. The parsing is divided in different steps.
 * 
 * - READ_INPUT: Reads user input stopping at a newline.
 * 
 * - TOKENIZATION: Breaks the input line into tokens (words, operators).
 * 
 * - VARIABLE_EXPANSION: Replaces and expands all $VAR on every token.
 * 
 * - COMAND_CASTING: Creats a list of commands by analizing the token list.
 * 
 * - REDIRECTIONS: Opens and close all redirections made with respective operators 
 * within each individual command on the list.
 * 
 * - OPERATOR_REMOVAL: Removes syntax quotes, \\ and ;.
 * 
 * @param minishell A pointer to the minishell enviroment structure.
 * @note If any error occurs during the parsing, the function will end with
 * a forcend([errno]) call.
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
