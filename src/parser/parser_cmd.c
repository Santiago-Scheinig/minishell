/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:01:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/13 18:26:35 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

static t_cmd	*cmdup(t_body *minishell, t_list *token_lst)
{
	t_cmd *new_cmd;

	if (!token_lst)
		sigend(minishell, 1);
	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		sigend(minishell, 1);
	memset(new_cmd, 0, sizeof(t_cmd));
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

static void	upd_cmd(t_token *aux, t_cmd *new)
{
	int	i;

	/*Im going to have argv already initalized but its innter pointers set to NULL*/
	/*Then, i can loop until NULL, once i did, thats the place where to save the WORD*/
	/*If its the first time, it will save the first valid WORD in position 0, setting the command*/
	i = 0;
	while (new->argv[i])
		i++;
	new->argv[i] = aux->str;
}

static void	save_cmd(t_body *minishell, t_cmd **aux, t_list *token_lst)
{
	t_token	*pipe;
	t_list	*new_node;

	new_node = ft_lstnew((*aux));
	if (!new_node)
		sigend(minishell, 1);
	ft_lstadd_back(&(minishell->cmd_lst), new_node);
	pipe = (t_token *) token_lst->content;
	if (pipe->type == PIPE)
		(*aux) = cmdup(minishell, token_lst);
	else
		(*aux) = NULL;
}

//i need to count the amount of arguments that exists by each pipe!!
//i think ill be doing it in save_cmd? and before starting the loop, but
//i need to trim some lines...
void	parser_cmds(t_body *minishell)
{
	t_list	*lst_aux;
	t_cmd	*new_cmd;
	t_token	*token_aux;
	t_token	*token_next;

	lst_aux = minishell->token_lst;
	new_cmd = cmdup(minishell, minishell->token_lst);//here, during creation of cmd.
	while (lst_aux)
	{
		token_aux = (t_token *) lst_aux->content;
		if (lst_aux->next)
			token_next = (t_token *) lst_aux->next->content;
		if (token_aux->type == REDIR_IN || token_aux->type == REDIR_OUT
			|| token_aux->type == REDIR_APPEND || token_aux->type == HEREDOC)
		{
			upd_redir(token_aux, token_next, new_cmd);
			lst_aux = lst_aux->next;
		}
		else if (token_aux->type == WORD)
			upd_cmd(token_aux, new_cmd);
		else if (token_aux->type == PIPE)
			save_cmd(minishell, &new_cmd, lst_aux->next);//then save cmd re executes create_cmd
		lst_aux = lst_aux->next;
	}
	save_cmd(minishell, &new_cmd, NULL);
}