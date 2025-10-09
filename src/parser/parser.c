/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/08 20:07:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_cmd.h"

static int	outfd_setdfl(void)
{
	int	tty_fd;
	int	orig_outfd;

	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd == -1)
		return (-1);
	orig_outfd = dup(STDOUT_FILENO);
	if (orig_outfd == -1)
	{
		close(tty_fd);
		return (-1);
	}
	if (!dup2(tty_fd, STDOUT_FILENO))
	{
		close(tty_fd);
		return (-1);
	}
	close(tty_fd);
	return (orig_outfd);
}

static int	outfd_restore(int orig_outfd)
{
	if (orig_outfd == -1)
		return (-1);
	if (dup2(orig_outfd, STDOUT_FILENO))
	{
		close(orig_outfd);
		return (-1);
	}
	close(orig_outfd);
	return (MSHELL_SUCCESS);
}

static void	parser_prompt(t_body *msh)
{
	char	*tmp;

	tmp = shell_pmtexp(msh->lst_t_var);
	if (!tmp)
		forcend(msh, "malloc", MSHELL_FAILURE);
	msh->input = readline(tmp);
	free(tmp);
	if (!msh->input && errno)
		forcend(msh, "readline", MSHELL_FAILURE);
}

/**
 * COMMENT PENDING
 */
static void	parser_input(t_body *msh)
{
	char	*tmp;
	int		orig_outfd;

	if (msh->interactive && isatty(STDOUT_FILENO))
		parser_prompt(msh);
	else if (msh->interactive)
	{
		orig_outfd = outfd_setdfl();
		parser_prompt(msh);
		outfd_restore(orig_outfd);
	}
	else
	{
		tmp = get_next_line(STDIN_FILENO);
		if (!tmp && errno == ENOMEM)
			forcend(msh, "malloc", MSHELL_FAILURE);
		else if (!tmp && errno)
			forcend(msh, "read", MSHELL_FAILURE);
		msh->input = ft_strtrim(tmp, "\n");
		if (tmp)
			free(tmp);
		msh->line++;
	}
	if (msh->input && !msh->input[0])
	{
		free(msh->input);
		msh->input = NULL;
		parser_input(msh);
	}
	if (!msh->input)
		msh_exit(NULL, msh);
}

/**
 * Analizes user input, validates it's syntax and saves a list of commands
 * to execute. The parsing is divided in different steps.
 * 
 * - READ_INPUT: Reads user input stopping at a newline.
 * 
 * - HERE_DOCUMENT: Asks the user to fill all the specified here_doc.
 * 
 * - TOKENIZATION: Breaks the input line into tokens (words, operators).
 * 
 * - VARIABLE_EXPANSION: Replaces and expands all $VAR on every token.
 * 
 * - COMAND_CASTING: Creats a list of commands by analizing the token list.
 * 
 * - REDIRECTIONS: Opens and closes all redirections made with respective 
 * operators within each individual command on the list.
 * 
 * - OPERATOR_REMOVAL: Removes syntax quotes, \\ and ;.
 * 
 * @param msh A pointer to the minishell enviroment structure.
 */
int	parser(t_body *msh)
{
	char	**split;

	cleanup(msh);
	parser_input(msh);
	if (msh->interactive && msh->input[0] != '\0')
		add_history(msh->input);
	split = shell_split(msh->input);
	if (!split)
		forcend(msh, "malloc", MSHELL_FAILURE);
	free(msh->input);
	msh->input = NULL;
	if (parser_token(split, msh))
	{
		if (msh->exit_no == MSHELL_MISSUSE && !msh->interactive)
			forcend(msh, NULL, msh->exit_no);
		if (msh->exit_no == MSHELL_FAILURE)
			forcend(msh, msh->exit_ft, msh->exit_no);
		else
			return (msh->exit_no);
	}
	parser_envar(msh);
	parser_cmds(msh);
	shell_lstclear(&(msh->lst_t_token), shell_lstdel_tkn);
	msh->lst_t_token = NULL;
	return (MSHELL_SUCCESS);
}
