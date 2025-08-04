/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/04 18:53:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <fcntl.h>
# include <unistd.h>

/**
 * An enumeration list of token types.
 */
typedef enum e_token_type
{
	WORD,			//	Command or argument
	PIPE,			//	|
	REDIR_IN,		//	<
	REDIR_OUT,		//	>
	REDIR_APPEND,	//	>>
	HEREDOC,		//	<<
	END,			//	End of line
}	t_token_type;

/**
 * A token structure where to save the value and it's type.
 */
typedef struct s_token
{
	char			*str;	//	The raw token string.
	t_token_type	type;	//	The type of token.
}	t_token;

/**
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*create_token(t_body *minishell, char *str);

void	update_redir(t_token *aux, t_token *next, t_cmd *new);

void	save_cmd(t_body *minishell, t_cmd **aux, t_list *token_lst);

void	sigend(t_body *minishell, int errno);

void	update_cmd(t_token *aux, t_cmd *new);

t_cmd	*create_cmd(t_body *minishell, t_list *token_lst);

#endif