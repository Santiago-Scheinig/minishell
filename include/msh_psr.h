/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:57:03 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/25 21:07:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_H
# define MSH_PSR_H

# include "lib/msh_std.h"
# include "lib/msh_tsh.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>

typedef struct s_envar_pair
{
	int	var;
	int	val;
}	t_envar_pair;

typedef struct s_hdoc_pair
{
	int fd;
	int	exit_no;
}	t_hdoc_pair;

typedef struct s_hdoc_tsh
{
	char	*limitator;
	int		line;
	int		heredoc[2];
}	t_hdoc_tsh;

/**
 * Divides user input into tokens catalogated as the enum structure 
 * T_TOKEN_TYPE indicates.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @param split The user input already divided with shell_split() parameters.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
int	parser_token(t_body *msh, char **split);

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
 * @param value A pointer to the STRING value of the enviromental variable.
 * @param start An index to the start position of the enviromental
 * variable on [str].
 * @return A pointer to the expanded STRING.
 * @note If value is NULL, no allocation is made and the original
 * [str] becomes cut removing the enviromental variable name of it.
 * Otherwise, it reallocates the new expanded string and returns
 * it.
 */
char	*exp_value(char *str, char *value, int start);

/**
 * Expands an enviromental variable mask and returs the modified value.
 * 
 * @param word A pointer to the WORD token where to expand.
 * @param start The position index of the enviromental variable on 
 * the WORD token string.
 * @param envar_len The length of the enviromental variable's name.
 * @param value_len The length of the enviromental variable's value.
 * @return A pointer to the expanded STRING.
 * @note If value_len is zero, no allocation is made and the original
 * [mask] becomes cut removing the enviromental variable name mask of it.
 * Otherwise, it reallocates the new expanded string and returns
 * it.
 */
char	*exp_mask(char *str, char *mask, int start, t_envar_pair len);

/**
 * Calculates the length of the enviroment variable name.
 * 
 * @param env_var A pointer to where the enviromental variable name start's.
 * @return The lenght of the enviroment variable name.
 * @note The minimum lenght size of a enviroment variable is always one,
 * on behalf of the '$' sign.
 */
int	envar_len(char *env_var);

/**
 * Analizes the WORD syntaxis and expands all enviromental variables avalible
 * inside of it following the quoting rules for expansion of enviromental
 * variables.
 * 
 * @param token_lst A pointer with the current position on the token_lst.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
int	envar_syntax(char **str, char **mask, t_list *envp, int exit_no);

int	exp_exitno(char **str, char **mask, int start, int exit_no);

/**
 * COMMENT UPDATE
 * Updates the WORD string mask to the new value of the declared enviromental 
 * variable if any, and expands it accordingly.
 * 
 * - If there's a value, the WORD token mask becomes reallocated and 
 * expanded with the same mask value.
 * 
 * - Otherwise, the WORD token mask string becomes cut from memory, without 
 * realocation, ereasing every character of the enviromental variable's mask.
 * 
 * @param word A pointer to the T_TOKEN to be expanded.
 * @param value A pointer to the STRING value of the enviromental variable.
 * @param start The position index of the enviromental variable on 
 * the WORD token string.
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @note If any error occurs during the tokenization step, the function will
 * end with a sigend([errno]) call.
 */
int	envar_mask(char *str, char *value, char **mask, int start);

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
char	*mask_dup(char *str);

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into heredoc[1].
 * 
 * @param limitator The string that will work as LIMITATOR.
 * @param heredoc An array of INT which saves an already initialized pipe()
 * @return Returns heredoc[0] from where to read everything that was 
 * written on heredoc[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on heredoc[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
int	heredoc_dup(t_token *limit, int heredoc[2], t_body *msh);

/**
 * COMMENT PENDING
 */
t_list	*cmdupd_err(t_list *aux_lst, t_cmd **new_cmd);

/**
 * COMMENT PENDING
 */
int	cmdupd_redir(t_token *aux, t_token *aux_next, t_cmd *new);

#endif