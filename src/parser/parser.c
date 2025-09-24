/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/24 19:53:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_cmd.h"

/**
 * COMMENT PENDING
 */
static void	parser_input(t_body *msh)
{
	char	*tmp;

	if (msh->interactive)
	{
		tmp = shell_pmtexp(msh->envp_lst);
		if (!tmp)
			forcend(msh, "malloc", MSHELL_FAILURE);
		msh->input = readline(tmp);
		free(tmp);
		if (errno)
			forcend(msh, "readline", MSHELL_FAILURE);
	}
	else
	{
		msh->input = ft_strtrim(get_next_line(STDIN_FILENO), "\n");
		if (errno == ENOMEM)
			forcend(msh, "malloc", MSHELL_FAILURE);
		else if (errno)
			forcend(msh, "read", MSHELL_FAILURE);
	}
	if (msh->input && !msh->input[0])
	{
		free(msh->input);
		parser_input(msh);
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
int	parser(t_body *msh)
{
	char	**split;

	cleanup(msh);
	parser_input(msh);
	if (msh->input == NULL)
		msh_exit(NULL, msh);
	if (msh->interactive && msh->input[0] != '\0')
		add_history(msh->input);
	split = shell_split(msh->input);
	if (!split)
		forcend(msh, "malloc", MSHELL_FAILURE);
	if (parser_token(msh, split))
	{
		if (msh->exit_no == MSHELL_FAILURE)
			forcend(msh, msh->exit_ft, msh->exit_no);
		else
			return (msh->exit_no);
	}
	msh->exit_no = MSHELL_SUCCESS;
	parser_envar(msh);
	shell_lstclear(&(msh->token_lst), shell_lstdeltkn);
	msh->token_lst = NULL;
	parser_cmds(msh);
	return (MSHELL_SUCCESS);
}
