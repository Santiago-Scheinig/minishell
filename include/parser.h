/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/07/25 19:13:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

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
	t_token			*next;	//	The next token in the list
}	t_token;

/**
 * Creates and allocates a new LIST node.
 * @param content A pointer to the content to be included inside of the new
 * node.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*new_token(t_body *minishell, char	*content)

/**
 * Adds a lst node at the end of a lst HEAD.
 * @param lst The LIST HEAD where to add the new node.
 * @param new The new LIST node to add.
 * @note If the LIST HEAD is empty, sets the new LIST node at the start
 * of it.
 */
void	token_add_back(t_token **lst, t_token *new)
#endif