/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 14:46:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "bicmd.h"


/*static void	print_export(t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;

	current = env_lst;
	while (current)
	{
		tmp = (t_var *)current->content;
		if (tmp->name && tmp->exported)
		{
			ft_printf("declare -x %s", tmp->name);
			if (tmp->value)
				ft_printf("|=\"%s\"", tmp->value);
			ft_printf("\n");
		}
		current = current->next;
	}
}*/


void	parser_input(t_body *minishell)
{
	char	*tmp;

	if (minishell->interactive)
	{
		tmp = shell_prompt(minishell);
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
 * COMMENT PENDING
 */
void	new_prompt(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * COMMENT PENDING
 */
void	parser_input(t_body *minishell)
{
	if (minishell->interactive)
	{
		if (!shell_prompt(minishell))
			forcend(minishell, "malloc", MSHELL_FAILURE);
		minishell->input = readline(minishell->prompt);
	}
	else
	{
		minishell->input = get_next_line(STDIN_FILENO);
		minishell->line++;
	}
	if (!minishell->input)
		forcend(minishell, NULL, MSHELL_SUCCESS);
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
