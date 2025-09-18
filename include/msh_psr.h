/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:28:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_H
# define MSH_PSR_H

# include "lib/msh_std.h"
# include "lib/msh_tsh.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

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
 * 
 * @param minishell A pointer to the main enviroment structure of minishell
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
void	parser_envar(t_body *minishell);

void	parser_cmds(t_body *minishell);

t_list	*cmd_rerr(t_list *aux_lst, t_cmd **new_cmd);

/**
 * Creates and allocates a new T_TOKEN node.
 * @param str A pointer to the STRING to be tokenized.
 * @return A pointer to the new T_TOKEN allocation; or NULL in case of error.
 */
t_token	*token_dup(char *str);

/**
 * Expands an enviromental variable and returs the modified value.
 * 
 * @param str A pointer to the WORD string where to expand.
 * @param value A pointer to the STRING to be expanded.
 * @param start An index to the start position of the enviromental
 * variable on [str].
 * @return A pointer to the expanded STRING.
 * @note If value is NULL, no allocation is made and the original
 * [str] becomes cut removing the enviromental variable name of it.
 * Otherwise, it reallocates the new expanded string and returns
 * it.
 */
char	*exp_value(char *str, char *value, int start);

char	*exp_mask(t_token *word, int start, int var_len, int value_len);

/**
 * Calculates the length of the enviroment variable name.
 * 
 * @param env_var A pointer to where the enviromental variable name start's.
 * @return The lenght of the enviroment variable name.
 * @note The minimum lenght size of a enviroment variable is always one,
 * on behalf of the '$' sign.
 */
int	envar_len(char *env_var);

int	edit_infile(t_token *next, t_cmd *new);
int	edit_outfile(t_token *next, t_cmd *new, int open_flag);
int	edit_infile_to_heredoc(t_token *next, t_cmd *new);

/**
 * Allocates and returns a clean STRING with only the enviromental variable
 * name, to search into getenv().
 * 
 * @param env_var A pointer to the WORD string on the position where the
 * enviromental variable name starts (Position after the '$').
 * @return A pointer to a new allocated STRING that only includes the name
 * of the enviromental variable.
 */
char	*envar_pathname(char *env_var);

/**
 * COMMENT PENDING
 */
t_list	*cmdupd_err(t_list *aux_lst, t_cmd **new_cmd);

/**
 * COMMENT PENDING
 */
int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new);

#endif