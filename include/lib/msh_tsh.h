/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_tsh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:16:00 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 20:25:31 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TSH_H
# define MSH_TSH_H

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

/**
 * Interpects the SIGINT signal and executes a signal handler.
 * 
 * The intercepted signal indicates the program to execute new_prompt().
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @return A pointer to the main enviroment structure of minishell, 
 * or NULL if the interception failed.
 * @note Additionally, it blocks both SIGINT and SIGQUIT signals during
 * the handler's execution to prevent nested signals from interfering. Then
 * uses SA_RESTART to automatically restart interrupted syscalls.
 */
int	sigint(void);

/**
 * Intercepts the SIGQUIT signal and executes a signal handler.
 * 
 * The intercepted signal is indicated to SIG_IGN, which tells the program 
 * to ignore it entirely.
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @return A pointer to the main enviroment structure of minishell, 
 * or NULL if the interception failed.
 * @note SIGQUIT is typically used to quit a process and produce a core dump.
 */
int	sigquit(void);

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

int		forcend(t_body *minishell, char *function, t_error number);

int		redirend(char *argv, t_error number);

int		sigend(const char *next, t_error number, t_body *minishell);

void	end_minishell(t_body *minishell);

#endif