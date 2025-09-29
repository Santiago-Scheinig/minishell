/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:54:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/29 12:28:51 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_tsh.h"

/**
 * Global flag use to track signals.
 * 
 * This flag is used to notify the shell that the signal was intercepted,
 * allowing proper prompt refresh and cleanup.
 * 
 * @note Declared volatile because it is modified from within a signal handler.
 */
volatile sig_atomic_t	g_signal_received = 0;

/**
 * Signal handler to refresh the interactive prompt after an interrupt (SIGINT)
 * 
 * @param signum The received signal number (typically SIGINT).
 * 
 * Sets the global signal flag to indicate an interrupt was received, writes a
 * newline to keep the prompt on a new line, clears the current readline buffer
 * and forces readline to redisplay a fresh prompt.
 * 
 * @note - This handler is intended for interactive prompt context only.
 * - Uses async-signal-safe functions where applicable.
 */
void	new_prompt(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * Installs signal handlers that ignore SIGINT and SIGQUIT.
 * 
 * @return MSHELL_SUCCESS on success, MSHELL_FAILURE if sigaction() fails.
 * 
 * Configures SIGINT and SIGQUIT to be ignored by setting their handler to
 * SIG_IGN. The function initializes an empty mask, adds SIGINT and SIGQUIT
 * to the mask, and uses SA_RESTART so interrupted system calls will be
 * automatically restarted where supported.
 * 
 * @note - Intended for phases where the shell should ignore user-generated
 *         interrupt/quit signals.
 */
int	sigign(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("Error setting SIGINT handler");
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * Sets the SIGINT and SIGQUIT signal handler to default.
 * 
 * @return 0 on success, 1 if a sigaction() error occured.
 */
int	sigdfl(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) || sigaction(SIGQUIT, &sa_int, NULL)
		|| sigaction(SIGPIPE, &sa_int, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

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
int	sigint(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = new_prompt;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		perror("Error setting SIGINT handler");//not sure this should print like this
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * Sets the SIGQUIT signal handler to SIG_IGN.
 * 
 * The intercepted signal becomes ignored entirely.
 * 
 * @return 0 on success, 1 if a sigaction() error occured.
 */
int	sigquit(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("Error setting SIGQUIT handler");//not sure this should print like this
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}
