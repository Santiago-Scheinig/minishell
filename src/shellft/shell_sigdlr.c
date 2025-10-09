/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sigdlr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:54:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 02:55:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

/**
 * @brief Global flag indicating an interrupt signal was received.
 *
 * This variable is set to 1 by the SIGINT handler (set_flag) when the
 * shell is interrupted (e.g., Ctrl+C) in non-interactive mode. Other
 * parts of the shell check this flag to perform cleanup or abort
 * processing safely.
 *
 * @note	Declared volatile to ensure correct behavior in signal handlers.
 * @note	Declared sig_atomic_t to guarantee atomic reads/writes in
 *			signal handlers.
 */
volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief	Restores default handling for SIGINT and SIGQUIT signals.
 *
 *			Sets up a sigaction structure with SIG_DFL as the handler and an
 *			empty signal mask. Calls sigaction() for both SIGINT and SIGQUIT
 *			to restore their default behavior.
 *
 * @note	Use this when the shell needs standard signal behavior, e.g.,
 *			before executing child processes.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if any sigaction call fails.
 */
int	shell_sigdfl_all(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int, NULL) || sigaction(SIGQUIT, &sa_int, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Ignores all common interactive signals (SIGINT and SIGQUIT).
 *
 *			Sets up a sigaction structure with SIG_IGN as the handler for
 *			SIGINT. Adds both SIGINT and SIGQUIT to the signal mask and
 *			sets the SA_RESTART flag. Calls sigaction() to apply the settings.
 *
 * @note	Prevents the shell from being interrupted or quit by Ctrl+C or
 *			Ctrl+\ during critical operations.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if sigaction fails.
 */
int	shell_sigign_all(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Sets up a SIGINT handler that updates the global signal flag.
 *
 *			Initializes a sigaction structure with sigdlr_setflg as the
 *			handler. Adds SIGQUIT and SIGINT to the signal mask and sets
 *			the SA_RESTART flag. Calls sigaction() to apply the configuration.
 *
 * @note	Allows the shell to track when Ctrl+C is pressed without exiting.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if sigaction fails.
 */
int shell_sigint_setflg(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigdlr_setflg;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Sets up a custom SIGINT handler for the shell prompt.
 *
 *			Initializes a sigaction structure with sigdlr_newpmt as the
 *			handler. Adds SIGQUIT and SIGINT to the signal mask and sets
 *			the SA_RESTART flag. Calls sigaction() to apply the configuration.
 *
 * @note	Allows the shell to handle Ctrl+C gracefully without terminating.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE if sigaction fails.
 */
int	shell_sigint_newpmt(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigdlr_newpmt;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Configures the shell to ignore the SIGQUIT signal (Ctrl+\).
 *
 *			Sets up a sigaction structure with SIG_IGN as the handler and
 *			an empty signal mask. Calls sigaction() to apply the settings.
 *			If sigaction fails, returns MSHELL_FAILURE; otherwise returns
 *			MSHELL_SUCCESS.
 *
 * @note	Prevents the shell from quitting when the user types Ctrl+\.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on error.
 */
int	shell_sigquit(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}
