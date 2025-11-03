/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:22:35 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * @brief	Processes heredoc input respecting quote expansion rules.
 *
 *			Checks the mask string to detect presence of single or
 *			double quotes ('S' or 'D'). If found, disables variable
 *			expansion. Otherwise, applies envar_syntax() to expand
 *			environment variables inside the line. Then writes the
 *			resulting string to the heredoc file descriptor.
 *
 *			On any failure, returns MSHELL_FAILURE.
 *
 * @param	mask	Mask string associated with heredoc limit token.
 * @param	line	Pointer to current input line string.
 * @param	envar	Linked list of environment variables.
 * @param	hd		Structure containing heredoc fd and exit code.
 *
 * @note	Caller must free *line after this function returns.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on error.
 */
static int	hdoc_mask(char *mask, char **line, t_list *envar, t_hdoc_pair hd)
{
	int	i;
	int	exp;

	i = -1;
	exp = 1;
	while (mask[++i])
		if (mask[i] == 'S' || mask[i] == 'D')
			exp = 0;
	if (exp)
		if (envar_syntax(hd.exit_no, line, NULL, envar))
			return (MSHELL_FAILURE);
	if (write(hd.fd, (*line), ft_strlen((*line))) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Creates heredoc input and writes user-provided lines.
 *
 *			Uses ft_get_next_line() to read lines until the limiter
 *			token string is matched. Each line is processed through
 *			hdoc_mask() for possible variable expansion and written
 *			to the heredoc pipe's write end.
 *
 *			On write or read errors, invokes shell_hdocerr() and
 *			closes pipe descriptors safely.
 *
 *			Static helpers:
 *
 *				- hdoc_mask():	Handles heredoc expansion and output.
 *
 * @param	heredoc_fd	Pipe file descriptors [read, write].
 * @param	limit		Token containing the heredoc limiter string.
 * @param	msh			Pointer to main shell structure (t_body).
 *
 * @note	Closes the write end before returning the read end.
 *
 * @return	Read end file descriptor for heredoc content.
 */
static int	hdoc_dup(int heredoc_fd[2], t_token *limit, t_body *msh)
{
	int			strlen;
	char		*line;
	t_hdoc_pair	hd_utils;

	hd_utils.fd = heredoc_fd[1];
	hd_utils.exit_no = msh->exit_no;
	strlen = ft_strlen(limit->str) + 1;
	shell_memmove(limit->str, limit->str, limit->mask, strlen);
	while (1 && ++msh->line)
	{
		if (msh->interactive && write(STDERR_FILENO, "> ", 2) == -1)
			return (shell_hderr(limit->str, "write", heredoc_fd, msh));
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			return (shell_hderr(limit->str, "read", heredoc_fd, msh));
		if (line && ft_strlen(line) != 1
			&& !ft_strncmp(line, limit->str, ft_strlen(line) - 1))
			break ;
		if (hdoc_mask(limit->mask, &line, msh->head_envar, hd_utils))
			return (shell_hderr(limit->str, "read", heredoc_fd, msh));
		free(line);
	}
	free(line);
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}

/**
 * @brief	Handles heredoc creation for a heredoc-type token.
 *
 *			If the current token is HEREDOC and the following token
 *			is a WORD, creates a pipe to store heredoc content, then
 *			reads user input via hdoc_dup(). Properly sets signal
 *			handlers during heredoc execution.
 *
 *			On syntax or system call errors, reports and updates
 *			msh->exit_no accordingly.
 *
 *			Static helpers:
 *
 *				- hdoc_dup():	Performs heredoc input and writing.
 *
 * @param	aux_token	Current token (expected to be HEREDOC).
 * @param	next_token	Next token containing limiter value.
 * @param	msh			Pointer to main shell structure (t_body).
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on error.
 */
static int	token_hdoc(t_token *aux_token, t_token *next_token, t_body *msh)
{
	int	aux_fd[2];

	if (next_token->type != WORD)
		return (shell_parserr(next_token->str, msh));
	if (!msh->exit_no && aux_token->type == HEREDOC)
	{
		if (pipe(aux_fd) == -1)
		{
			aux_token->heredoc_fd = -1;
			msh->exit_no = MSHELL_FAILURE;
			shell_redirerr(msh->exit_no, NULL);
			return (MSHELL_SUCCESS);
		}
		if (shell_sigign_all())
			shell_forcend(MSHELL_FAILURE, "sigaction", msh);
		aux_token->heredoc_fd = hdoc_dup(aux_fd, next_token, msh);
		if (shell_sigint(msh->interactive) || shell_sigquit())
			shell_forcend(MSHELL_FAILURE, "sigaction", msh);
		if (aux_token->heredoc_fd == -1)
		{
			msh->exit_no = MSHELL_FAILURE;
			return (MSHELL_FAILURE);
		}
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Validates token sequence for syntax and heredoc rules.
 *
 *			Iterates through token list ensuring correct ordering of
 *			pipes and redirection operators. For heredocs, invokes
 *			token_hdoc() to build content. Detects invalid operator
 *			sequences like consecutive pipes or leading pipes.
 *
 * @param	head_token	Linked list head of tokenized commands.
 * @param	msh			Pointer to main shell structure (t_body).
 *
 * @note	Updates msh->exit_no during pipe handling and errors.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on syntax error.
 */
static int	token_syntax(t_list *head_token, t_body *msh)
{
	t_token	*aux_token;
	t_token	*next_token;
	int		i;

	i = 0;
	while (head_token->next)
	{
		aux_token = (t_token *) head_token->content;
		next_token = (t_token *) head_token->next->content;
		if (aux_token->type == REDIR_IN || aux_token->type == REDIR_OUT
			|| aux_token->type == REDIR_APPEND || aux_token->type == HEREDOC)
			if (token_hdoc(aux_token, next_token, msh))
				return (MSHELL_FAILURE);
		if ((aux_token->type == PIPE && next_token->type == PIPE))
			return (shell_parserr(next_token->str, msh));
		if ((!i++ && aux_token->type == PIPE))
			return (shell_parserr(aux_token->str, msh));
		else if (aux_token->type == PIPE)
			msh->exit_no = MSHELL_SUCCESS;
		head_token = head_token->next;
	}
	if (((t_token *) head_token->content)->type != WORD)
		return (shell_parserr(NULL, msh));
	return (0);
}

/**
 * @brief	Converts tokenized strings into a linked token list.
 *
 *			Creates new token objects from each split string using
 *			shell_newtkn() and links them in msh->head_token. Then
 *			performs syntax validation via token_syntax().
 *
 *			On allocation failure, frees remaining data and exits.
 *
 *			Static helpers:
 *
 *				- token_syntax():	Performs full token validation.
 *
 * @param	split	Array of token strings (from parser_input()).
 * @param	msh		Pointer to main shell structure (t_body).
 *
 * @note	Frees split array after processing.
 *
 * @return	Result of token_syntax(): 0 on success, non-zero on error.
 */
int	parser_token(char **split, t_body *msh)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	msh->head_token = NULL;
	while (split[++i])
	{
		new_token = shell_newtkn(split[i], OPERATORS);
		if (!new_token)
			break ;
		new_node = ft_lstnew(new_token);
		if (!new_node)
			break ;
		ft_lstadd_back(&(msh->head_token), new_node);
	}
	if (!new_node || !new_token)
	{
		if (new_token)
			free(new_token);
		ft_split_free(&split[i]);
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	}
	free(split);
	return (token_syntax(msh->head_token, msh));
}
