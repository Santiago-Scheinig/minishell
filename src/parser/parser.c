/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/17 16:40:46 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"
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

	sortenv(&minishell->envp_lst);
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
