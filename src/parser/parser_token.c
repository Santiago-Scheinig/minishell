/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 02:03:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	token_heredoc(t_token *aux_token, t_token *next_token, t_body *msh)
{
	int	aux_fd[2];

	if (next_token->type != WORD)
		return (parsend(next_token->str, MSHELL_MISSUSE, msh));
	if (!msh->exit_no && aux_token->type == HEREDOC)
	{
		if (pipe(aux_fd) == -1)
		{
			aux_token->heredoc_fd = -1;
			msh->exit_no = MSHELL_FAILURE;
			redirend(NULL, msh->exit_no);
			return (MSHELL_SUCCESS);
		}
		if (shell_sigign())
			forcend(msh, "sigaction", MSHELL_FAILURE);
		aux_token->heredoc_fd = heredoc_dup(next_token, aux_fd, msh);
		if (shell_sigint() || shell_sigquit())
			forcend(msh, "sigaction", MSHELL_FAILURE);
		if (aux_token->heredoc_fd == -1)
		{
			msh->exit_no = MSHELL_FAILURE;
			return (MSHELL_FAILURE);
		}
	}
	return (MSHELL_SUCCESS);
}

/**
 * Verifies the correct syntax of the user input inside a T_TOKEN list
 * by following the bash-shell rules:
 * 
 * - The list cannot beggin with a '|' OPERATOR.
 * 
 * - Any redirection OPERATOR must be followed by it's linked WORD.
 * 
 * - There cannot be two consecutive '|' OPERATORS.
 * 
 * - The list must allways end with a WORD.
 * 
 * @param msh A pointer to the main enviroment structure of minishell.
 */
static int	token_syntax(t_list *lst_t_token, t_body *msh)
{
	t_token	*aux_token;
	t_token	*next_token;
	int		i;

	i = 0;
	while (lst_t_token->next)
	{
		aux_token = (t_token *) lst_t_token->content;
		next_token = (t_token *) lst_t_token->next->content;
		if (aux_token->type == REDIR_IN || aux_token->type == REDIR_OUT
			|| aux_token->type == REDIR_APPEND || aux_token->type == HEREDOC)
			if (token_heredoc(aux_token, next_token, msh))
				return (MSHELL_FAILURE);
		if ((aux_token->type == PIPE && next_token->type == PIPE))
			return (shell_parsend(next_token->str, msh));
		if ((!i++ && aux_token->type == PIPE))
			return (shell_parsend(aux_token->str, msh));
		else if (aux_token->type == PIPE)
			msh->exit_no = MSHELL_SUCCESS;
		lst_t_token = lst_t_token->next;
	}
	if (((t_token *) lst_t_token->content)->type != WORD)
		return (shell_parsend(NULL, msh));
	return (0);
}

/**
 * @brief Determines the type of a shell token from its string value.
 *
 * Compares the input string to known shell operators and returns the
 * corresponding token type. If no operator matches, the token is
 * classified as a WORD. Empty or NULL strings return END.
 *
 * @param str  The token string to evaluate.
 *
 * @return  The t_token_type of the string (WORD, PIPE, REDIR_IN, etc.).
 */
static int	get_token_type(char *str)
{
	if (!str || !str[0])
		return (END);
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	return (WORD);
}

/**
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param msh A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 */
int	parser_token(char **split, t_body *msh)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	i = -1;
	msh->lst_t_token = NULL;
	if (!split[0])
		return (MSHELL_SUCCESS);
	while (split[++i])
	{
		new_token = token_dup(split[i]);
		if (!new_token)
			break ;
		new_node = ft_lstnew(new_token);
		if (!new_node)
			break ;
		ft_lstadd_back(&(msh->lst_t_token), new_node);
	}
	if (!new_node || !new_token)
	{
		ft_split_free(&split[i]);
		forcend(msh, "malloc", MSHELL_FAILURE);
	}
	free(split);
	return (token_syntax(msh->lst_t_token, msh));
}
