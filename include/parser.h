/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/14 20:05:16 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shellft.h"
# include "minishell.h"
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
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call,
 */
void	parser_token(t_body *minishell, char **split);

/**
 * Verifies if any WORD token includes a valid '$' OPERATOR. If it does, 
 * it expands it inside the T_TOKEN list following the quoting rules for
 * expansion of enviromental variables:
 * 
 * - Single quote enclosing: The enviromental variable remains as plain text.
 * 
 * - Double quote enclosing: The enviromental variable expands, but remains as
 * part of the WORD it was enclosed into.
 * 
 * - No quote enclosing: The enviromental variable expands and becomes 
 * tokenized into WORDS divided only by ' ' (OPERATORS are treated as plain
 * text after expansion).
 */
void	parser_envar(t_body *minishell);

void	parser_cmds(t_body *minishell);

/**
 * Creates and allocates a new T_TOKEN node.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param content A pointer to the STRING to be tokenized.
 * @return A pointer to the new LIST node; or NULL in case of error.
 * @note The next node inside of the LIST node is set to NULL.
 */
t_token	*token_dup(char *str);

int	addlst_here(t_list *token_list, char *str, int start);

char	*exp_value(char *str, int start, char *value);

char	*envar_pathname(char *env_var);

void	upd_redir(t_token *aux, t_token *next, t_cmd *new);

void	sigend(t_body *minishell, int errno);

#endif