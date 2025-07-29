/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 18:41:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/29 21:36:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

t_cmd	*create_cmd(t_body *minishell, t_list *token_lst)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd *));
	if (!new_cmd)
		sigend(minishell, 1);
	memset(new_cmd, 0, sizeof(t_cmd *));
	new_cmd->outfile = 1;
	new_cmd->heredoc[0] = -1;
	new_cmd->heredoc[1] = -1;
	//count arguments of token list. Its separated, so if pipes i send another position of the token_list.
	//new_cmd->argv = malloc((argument_len + 1)* sizeof(char *));
	//if (!new_cmd->argv)
	//	sigend(minishell, 1); But sigend will be a hell of complicated function, is good im NULL already freed str on redir.
	//memset(new_cmd->argv, 0, sizeof(argument_len * sizeof(char *)));
	//All the positions of the new_cmd->argv will be pointers already allocated.
	return (new_cmd);
}

void	update_cmd(t_token *aux, t_cmd *new)
{
	int	i;

	/*Im going to have argv already initalized but its innter pointers set to NULL*/
	/*Then, i can loop until NULL, once i did, thats the place where to save the WORD*/
	/*If its the first time, it will save the first valid WORD in position 0, setting the command*/
	i = 0;
	while (new->argv[i])
		i++;
	new->argv[i] = aux.str;
}

void	save_cmd(t_body *minishell, t_cmd **aux, t_list *token_lst)
{
	t_token	*pipe;
	t_list	*new_node;

	new_node = ft_lstnew((*aux));
	if (!new_node)
		sigend(minishell, 1);
	ft_lstadd_back(&(minishell->cmd_lst), new_node);
	aux = (t_token *) token_lst->content;
	if (pipe->type == PIPE)
		(*aux) = create_cmd(minishell, token_lst);
	else
		(*aux) = NULL;
}

static int	get_token_type(char *str)
{
	if (!str)
		return (END);
	if (!ft_strncmp(str, "|", 2))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 3))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 3))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 2))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 2))
		return (REDIR_OUT);
	return (WORD);
}

/**
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*create_token(t_body *minishell, char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		sigend(minishell, 1);
	new->str = str;
	new->type = get_token_type(str);
	return (new);
}
