/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/12 17:29:29 by sscheini         ###   ########.fr       */
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

void	*lstadd_exp(t_list **token_lst, char **split);

char	*envar_pathname(char *env_var);

int		envar_len(char *env_var);

void	tokenize(t_body *minishell, char **split);

void	get_envar(t_list *token_lst, t_body *minishell);

void	get_cmds(t_body *minishell);

void	upd_redir(t_token *aux, t_token *next, t_cmd *new);

void	sigend(t_body *minishell, int errno);

#endif