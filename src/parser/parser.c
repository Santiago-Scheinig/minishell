/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/29 19:30:16 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"
#include "parser.h"

/*This will likely have to be modified*/
/*Should we print the new line here??*/
void	new_prompt(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	parser_input(t_body *minishell)
{
	if (minishell->interactive)
	{
		if (shell_prompt(minishell))
			forcend(minishell, "malloc", MSHELL_FAILURE);
		minishell->input = readline(minishell->prompt);
	}
	else
		minishell->input = get_next_line(STDIN_FILENO);
	if (minishell->input == NULL)
	{
		shell_lstclear(&minishell->envp_lst, shell_lstdelvar);
		ft_split_free(minishell->envp);
		if (minishell->prompt)
			rl_clear_history();
		forcend(minishell, NULL, MSHELL_SUCCESS);
	}
	else if (!minishell->input[0])
		parser_input(minishell);
	else if (minishell->interactive && minishell->input[0] != '\0')
		add_history(minishell->input);
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

	cleanup(minishell);//cleanup comes first. Which the first time wont do shit.
	parser_input(minishell);
	split = shell_split(minishell->input);
	if (!split)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	parser_token(minishell, split);
	//verify global variable cuz of SIGUSR1, return if true!
	parser_envar(minishell);
	parser_cmds(minishell);
	shell_lstclear(&(minishell->token_lst), shell_lstdeltkn);
	minishell->token_lst = NULL;
}
