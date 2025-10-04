/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:02:55 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:16:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	token_heredoc(t_token *aux, t_token *next, t_body *msh)
{
	int	fd[2];

	if (next->type != WORD)
		return (parsend(next->str, MSHELL_MISSUSE, msh));
	if (!msh->exit_no && aux->type == HEREDOC)
	{
		if (pipe(fd) == -1)
		{
			aux->heredoc_fd = -1;
			msh->exit_no = MSHELL_FAILURE;
			redirend(NULL, msh->exit_no);
			return (MSHELL_SUCCESS);
		}
		shell_sigign();
		aux->heredoc_fd = heredoc_dup(next, fd, msh);
		shell_sigint();//forcend
		shell_sigquit();//forcend
		if (aux->heredoc_fd == -1)
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
	t_token	*aux;
	t_token	*next;
	int		i;

	i = 0;
	while (lst_t_token->next)
	{
		aux = (t_token *) lst_t_token->content;
		next = (t_token *) lst_t_token->next->content;
		if (aux->type == REDIR_IN || aux->type == REDIR_OUT
			|| aux->type == REDIR_APPEND || aux->type == HEREDOC)
			if (token_heredoc(aux, next, msh))
				return (msh->exit_no);
		if ((aux->type == PIPE && next->type == PIPE))
			return (parsend(next->str, MSHELL_MISSUSE, msh));
		if ((!i++ && aux->type == PIPE))
			return (parsend(aux->str, MSHELL_MISSUSE, msh));
		else if (aux->type == PIPE)
			msh->exit_no = MSHELL_SUCCESS;
		lst_t_token = lst_t_token->next;
	}
	if (((t_token *) lst_t_token->content)->type != WORD)
		return (parsend(NULL, MSHELL_MISSUSE, msh));
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
 * @brief Duplicates a string into a new t_token structure.
 *
 * Allocates a new t_token, sets its string to the provided str, and
 * determines its token type. If the type is WORD, also creates a mask
 * of the string using maskstr().
 *
 * @param str	Pointer to the string to store in the token.
 *
 * @note	Memory for t_token and mask (if applicable) is allocated;
 *			caller is responsible for freeing them.
 * @note	Returns NULL if allocation fails for t_token or mask.
 *
 * @return	Pointer to the newly allocated t_token on success, NULL on
 *			allocation failure.
 */
static t_token	*token_dup(char *str)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(t_token));
	new->str = str;
	new->type = get_token_type(str);
	if (new->type == WORD)
	{
		new->mask = mask_dup(str);
		if (!new->mask)
			return (NULL);
	}
	return (new);
}

/**
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param msh A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 */
int	parser_token(t_body *msh, char **split)
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
