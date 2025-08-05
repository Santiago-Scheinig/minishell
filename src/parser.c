/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:56:26 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/05 17:42:27 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

//i need to count the amount of arguments that exists by each pipe!!
//i think ill be doing it in save_cmd? and before starting the loop, but
//i need to trim some lines...
static void	get_cmds(t_body *minishell)
{
	t_list	*lst_aux;
	t_cmd	*new_cmd;
	t_token	*token_aux;
	t_token	*token_next;

	lst_aux = minishell->token_lst;
	new_cmd = create_cmd(minishell, minishell->token_lst);//here, during creation of cmd.
	while (lst_aux)
	{
		token_aux = (t_token *) lst_aux->content;
		if (lst_aux->next)
			token_next = (t_token *) lst_aux->next->content;
		if (token_aux->type == REDIR_IN || token_aux->type == REDIR_OUT
			|| token_aux->type == REDIR_APPEND || token_aux->type == HEREDOC)
		{
			update_redir(token_aux, token_next, new_cmd);
			lst_aux = lst_aux->next;
		}
		else if (token_aux->type == WORD)
			update_cmd(token_aux, new_cmd);
		else if (token_aux->type == PIPE)
			save_cmd(minishell, &new_cmd, lst_aux->next);//then save cmd re executes create_cmd
		lst_aux = lst_aux->next;
	}
	save_cmd(minishell, &new_cmd, NULL);
}

static void	verify_tokens(t_body *minishell)
{
	t_list	*lst_aux;
	t_token	*token_aux;
	t_token	*token_next;
	int		i;

	i = 0;
	lst_aux = minishell->token_lst;
	while (lst_aux->next)
	{
		token_aux = (t_token *) lst_aux->content;
		if (lst_aux->next)
			token_next = (t_token *) lst_aux->next->content;
		if (token_aux->type == REDIR_IN || token_aux->type == REDIR_OUT
			|| token_aux->type == REDIR_APPEND || token_aux->type == HEREDOC)
			if (token_next->type != WORD)
				sigend(minishell, 2);
		if (token_aux->type == PIPE && token_next->type == PIPE)
			sigend(minishell, 2);
		if (!i && token_aux->type == PIPE)
			sigend(minishell, 2);
		lst_aux = lst_aux->next;
		i++;
	}
	if (token_aux->type != WORD)
		sigend(minishell, 2);
}

static void	tokenize(t_body *minishell, char **split)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	minishell->token_lst = NULL;
	while(split[++i])
	{
		new_token = create_token(minishell, split[i]);
		new_node = ft_lstnew(new_token);
		if (!new_node)
			sigend(minishell, 1);
		ft_lstadd_back(&(minishell->token_lst), new_node);
	}
	free(split);
}

/**
 * change the split to a specific shell iq_token_split (which ignores whatever
 * is inside quotes (double and single), as well as unclosed quotes)
 */
void	parser(t_body *minishell, char *input)
{
	char	**split;

	split = shell_split(input);
	free(input);
	if (!split)
		sigend(minishell, 1);
	int i = -1;
	while (split[++i])
		printf("%s\n",split[i]);
	return ;
	tokenize(minishell, split);
	verify_tokens(minishell);
	return ;
	//i should verify this work before keep going.
	get_cmds(minishell);
	ft_lstclear(&(minishell->token_lst), free);
	minishell->token_lst = NULL;
}
