/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:27:05 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * @brief	Counts the number of arguments in a command segment.
 *
 *			Iterates through the token list starting from lst_token
 *			and counts consecutive WORD-type tokens until a PIPE is
 *			found. Used to preallocate argv space for a t_cmd.
 *
 * @param	lst_token	List of tokens starting from current command.
 *
 * @note	Stops counting when PIPE or END token is encountered.
 *
 * @return	Number of argument tokens found before the next PIPE.
 */
static int	cmd_argc(t_list *lst_token)
{
	t_token	*aux;
	int		prev_type;
	int		count;

	count = 0;
	prev_type = WORD;
	while (lst_token)
	{
		aux = (t_token *) lst_token->content;
		if (aux->type == PIPE)
			break ;
		if (aux->type == WORD && prev_type == WORD)
			count++;
		prev_type = aux->type;
		lst_token = lst_token->next;
	}
	return (count);
}

/**
 * @brief	Initializes a new command structure (t_cmd).
 *
 *			Allocates and zero-initializes a t_cmd object. Sets
 *			default file descriptors to standard input and output.
 *			Allocates argv array with sufficient space for arguments
 *			and a terminating NULL pointer.
 *
 *			Static helpers:
 *
 *				- cmd_argc():	Determines argv allocation size.
 *
 * @param	lst_token	First token of the command segment.
 *
 * @note	Returns NULL if allocation fails.
 *
 * @return	Pointer to initialized t_cmd structure or NULL.
 */
static t_cmd	*cmd_init(t_list *lst_token)
{
	t_cmd	*new_cmd;
	int		argc;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	ft_memset(new_cmd, 0, sizeof(t_cmd));
	new_cmd->infd = STDIN_FILENO;
	new_cmd->outfd = STDOUT_FILENO;
	argc = cmd_argc(lst_token) + 1;
	new_cmd->argv = malloc((argc) * sizeof(char *));
	if (!new_cmd->argv)
	{
		free(new_cmd);
		return (NULL);
	}
	ft_memset(new_cmd->argv, 0, (argc * sizeof(char *)));
	return (new_cmd);
}

/**
 * @brief	Adds a token string to a command's argv array.
 *
 *			Finds the first empty slot in argv and assigns aux->str.
 *			Uses shell_memmove() to normalize the string by applying
 *			the mask before storing. If the first argument is empty,
 *			the slot is left as NULL.
 *
 * @param	aux			Token whose string is to be added.
 * @param	new_cmd		Command being populated with arguments.
 *
 * @note	Sets aux->str to NULL to transfer string ownership.
 */
static void	cmdupd_argv(t_token *aux, t_cmd *new_cmd)
{
	int		i;

	i = 0;
	while (new_cmd->argv[i])
		i++;
	shell_memmove(aux->str, aux->str, aux->mask, ft_strlen(aux->str) + 1);
	if (!aux->str[0] && !i)
	{
		free(aux->str);
		new_cmd->argv[0] = NULL;
	}
	else
		new_cmd->argv[i] = aux->str;
	aux->str = NULL;
}

/**
 * @brief	Saves the current command and initializes the next one.
 *
 *			Creates a new list node containing *new_cmd and appends
 *			it to msh->lst_t_cmd. If lst_token is not NULL, prepares
 *			a new command structure for the next segment following a
 *			PIPE token.
 *
 *			Static helpers:
 *
 *				- cmd_init():	Allocates next command structure.
 *
 * @param	lst_token	Current token node (may be PIPE).
 * @param	new_cmd		Pointer to the active command structure.
 * @param	msh			Main shell structure.
 *
 * @note	Forces exit on allocation failure.
 */
static void	cmdupd_save(t_list *lst_token, t_cmd **new_cmd, t_body *msh)
{
	t_list	*new_node;

	new_node = ft_lstnew((*new_cmd));
	if (!new_node)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	ft_lstadd_back(&(msh->head_cmd), new_node);
	if (lst_token)
	{
		(*new_cmd) = cmd_init(lst_token->next);
		if (!(*new_cmd))
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
	}
}

/**
 * @brief	Builds command structures from the token list.
 *
 *			Iterates through all tokens and constructs a sequence of
 *			t_cmd structures representing piped commands. Handles
 *			redirections, argument insertion, and pipe boundaries.
 *
 *			Calls cmdupd_redir() for redirections, cmdupd_argv() for
 *			arguments, and cmdupd_save() when a PIPE token is found.
 *
 *			Static helpers:
 *
 *				- cmd_init():	Initializes new command blocks.
 *				- cmdupd_argv():	Adds arguments to command argv.
 *				- cmdupd_save():	Appends completed command nodes.
 *
 * @param	msh	Main shell structure (contains token list and cmd list).
 *
 * @note	Forces shell termination on any allocation failure.
 */
void	parser_cmds(t_body *msh)
{
	t_cmd	*new_cmd;
	t_list	*lst_token;
	t_token	*tkn;

	lst_token = msh->head_token;
	new_cmd = cmd_init(msh->head_token);
	if (!new_cmd)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	while (lst_token)
	{
		tkn = (t_token *) lst_token->content;
		if (tkn->type && tkn->type != PIPE && tkn->type != END)
		{
			lst_token = lst_token->next;
			msh->exit_no = cmdupd_redir(tkn, lst_token->content, new_cmd);
			if (msh->exit_no)
				lst_token = cmdupd_err(lst_token, &new_cmd);
		}
		else if (tkn->type == WORD)
			cmdupd_argv(tkn, new_cmd);
		else if (tkn->type == PIPE)
			cmdupd_save(lst_token, &new_cmd, msh);
		lst_token = lst_token->next;
	}
	cmdupd_save(NULL, &new_cmd, msh);
}
