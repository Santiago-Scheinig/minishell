/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:51:55 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/24 15:51:55 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <termios.h>
# include <signal.h>
# include <stdio.h>
# include "shellft.h"
# include "libft.h"
# include <errno.h>

extern volatile sig_atomic_t	g_signal_received;

/*Id like to change this to a envp structure, inside a t_list structure.*/
/*If the only functions that are able to edit this, are shell functions,*/
/*Then we put this inside of shellft.h instead, more clean.				*/
typedef struct t_var
{
	char				*value;
	char				*name;
	int					exported;
}	t_var;

/**
 * Struct used to save the enviroment variables of the minishell.
 * 
 * @param term			Termios structure linked to the terminal initialization
 * @param commands
 * @param input
 * @param childs_pid
 * @param errno
 */
typedef struct s_body
{
	struct termios	orig_term;
	int				shell_errno;
	int				interactive;
	char			**envp;//A copy of the original envp + post modifications
	char			*user;
	char			*input;//needed for history?
	char			*prompt;//A char * promt, not really needed to save.
	t_list			*cmd_lst;
	t_list			*envp_lst;
	t_list			*token_lst;
}	t_body;

void	new_prompt(int signum);

void	parser(t_body *minishell);

int		execmd(t_body *minishell);

int		shell_prompt(t_body *minishell);

char	*shell_getenv(t_list *lst_var, const char *name);

#endif
