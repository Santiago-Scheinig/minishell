/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_tsh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:16:00 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/30 21:29:13 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TSH_H
# define MSH_TSH_H

# include "lib/msh_std.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <errno.h>

/*--------------------------------------------------------------------------*/
/*------------------------------SIGNAL HANDLERS-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * Global flag use to track signals.
 * 
 * This flag is used to notify the shell that the signal was intercepted,
 * allowing proper prompt refresh and cleanup.
 * 
 * @note Declared volatile because it is modified from within a signal handler.
 */
extern volatile sig_atomic_t	g_signal_received;

void	new_prompt(int signum);

/**
 * Sets the SIGINT signal handler to execute new_prompt() on call.
 * 
 * The intercepted signal indicates the program to execute new_prompt().
 * 
 * @return 0 on success, 1 if a sigaction() error occured.
 * @note - It also blocks both SIGINT and SIGQUIT signals during the handler's
 * execution preventing nested signals. 
 * @note - The SA_RESTART flag automatically restart interrupted syscalls.
 */
int	sigint(void);

/**
 * Sets the SIGQUIT signal handler to SIG_IGN.
 * 
 * The intercepted signal becomes ignored entirely.
 * 
 * @return 0 on success, 1 if a sigaction() error occured.
 */
int	sigquit(void);

/**
 * Sets the SIGINT and SIGQUIT signal handler to default.
 * 
 * @return 0 on success, 1 if a sigaction() error occured.
 */
int	sigdfl(void);

int	sigign(void);


/*--------------------------------------------------------------------------*/
/*-----------------------------SHELL TROUBLESHOOT---------------------------*/
/*--------------------------------------------------------------------------*/

typedef enum e_error
{
	MSHELL_FATAL = -1,
	MSHELL_SUCCESS,
	MSHELL_FAILURE,
	MSHELL_MISSUSE,
	MSHELL_CMD_INVEXE = 126,
	MSHELL_CMD_NOTEXE,
	MSHELL_SIG_HANDLR,
}	t_error;

void	cleanup(t_body *minishell);

/**
 * COMENT PENDING
 */
int		forcend(t_body *msh, const char *argv, int exit_no);

int		redirend(char *argv, t_error number);

int		parsend(const char *next, t_error number, t_body *minishell);

void	end_minishell(t_body *minishell);

#endif