/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 20:36:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/20 16:04:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * @brief	Resets command data after a redirection error.
 *
 *			Frees argv and pathname from the current command and
 *			clears its structure. Then skips remaining tokens until
 *			the next PIPE, allowing parsing to continue cleanly for
 *			the following command segment.
 *
 * @param	lst_token	Current token in the token list.
 * @param	new_cmd		Double pointer to the active command struct.
 *
 * @note	Used after a redirection error to safely resume parsing.
 *
 * @return	Pointer to the token before the next PIPE or END token.
 */
t_list	*cmdupd_err(t_list *lst_token, t_cmd **new_cmd)
{
	t_token	*aux_tkn;

	if ((*new_cmd)->argv)
		ft_split_free((*new_cmd)->argv);
	if ((*new_cmd)->pathname)
		free((*new_cmd)->pathname);
	ft_memset((*new_cmd), 0, sizeof(t_cmd));
	while (lst_token->next)
	{
		aux_tkn = (t_token *) lst_token->next->content;
		if (aux_tkn->type == PIPE)
			return (lst_token);
		lst_token = lst_token->next;
	}
	return (lst_token);
}

/**
 * @brief	Updates command input descriptor for '<' redirection.
 *
 *			Opens the target file for reading and assigns its file
 *			descriptor to new->infd. If the file cannot be opened or
 *			accessed, sets infd to -1 and reports the error.
 *
 * @param	tkn_next	Token containing the filename for input.
 * @param	new			Command to receive the updated input fd.
 *
 * @note	Closes previous infd if it was greater than STDERR_FILENO.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE otherwise.
 */
static int	cmdupd_infile(t_token *tkn_next, t_cmd *new)
{
	if (new->infd > 2)
		close(new->infd);
	if (access(tkn_next->str, R_OK | F_OK))
	{
		new->infd = -1;
		return (shell_redirerr(MSHELL_FAILURE, tkn_next->str));
	}
	else
	{
		new->infd = open(tkn_next->str, O_RDONLY);
		if (new->infd < 0)
			return (shell_redirerr(MSHELL_FAILURE, tkn_next->str));
	}
	free(tkn_next->str);
	tkn_next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Updates command output descriptor for '>' or '>>'.
 *
 *			Opens (or creates) a file for writing with the specified
 *			open_flag. Handles both truncate and append modes, and
 *			assigns the resulting descriptor to new->outfd.
 *
 * @param	open_flag	File open mode (O_TRUNC or O_APPEND).
 * @param	tkn_next	Token containing the output filename.
 * @param	new			Command to receive the updated output fd.
 *
 * @note	Closes previous outfd if greater than STDERR_FILENO.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on error.
 */
static int	cmdupd_outfile(int open_flag, t_token *tkn_next, t_cmd *new)
{
	if (new->outfd > 2)
		close(new->outfd);
	new->outfd = open(tkn_next->str, O_WRONLY | O_CREAT | open_flag, 0664);
	if (new->outfd < 0)
		return (shell_redirerr(MSHELL_FAILURE, tkn_next->str));
	free(tkn_next->str);
	tkn_next->str = NULL;
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Assigns heredoc file descriptor to command input.
 *
 *			If heredoc_fd is valid, links it to new->infd. If the
 *			fd is invalid (-1), signals redirection failure.
 *
 * @param	aux		Token containing heredoc information.
 * @param	new		Command structure to update.
 *
 * @note	Closes previous infd if greater than STDERR_FILENO.
 *
 * @return	MSHELL_SUCCESS or MSHELL_FAILURE based on heredoc status.
 */
static int	cmdupd_heredoc(t_token *aux, t_cmd *new)
{
	if (new->infd > 2)
		close(new->infd);
	if (aux->heredoc_fd == -1)
		return (MSHELL_FAILURE);
	new->infd = aux->heredoc_fd;
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Handles all command redirection updates.
 *
 *			Determines the redirection type from aux->type and
 *			dispatches to the proper helper for handling input,
 *			output, append, or heredoc redirections.
 *
 *			Static helpers:
 *
 *				- cmdupd_infile():	Handles '<' input redirection.
 *				- cmdupd_outfile():	Handles '>' and '>>' output.
 *				- cmdupd_heredoc():	Handles heredoc linking.
 *
 * @param	aux			Current token (redir operator).
 * @param	aux_next	Next token (target filename or heredoc).
 * @param	new			Active command structure.
 *
 * @note	Returns failure if an unknown redirection type is found.
 *
 * @return	MSHELL_SUCCESS or MSHELL_FAILURE depending on result.
 */
int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new)
{
	if (aux->type == REDIR_IN)
		return (cmdupd_infile(aux_next, new));
	if (aux->type == REDIR_OUT)
		return (cmdupd_outfile(O_TRUNC, aux_next, new));
	if (aux->type == REDIR_APPEND)
		return (cmdupd_outfile(O_APPEND, aux_next, new));
	if (aux->type == HEREDOC)
		return (cmdupd_heredoc(aux, new));
	return (MSHELL_FAILURE);
}
