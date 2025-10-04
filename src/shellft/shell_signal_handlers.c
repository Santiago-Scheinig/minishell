/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal_handlers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:54:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:00:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"
#include "lib/msh_std_utils.h"

/**
 * @brief Global flag indicating that a signal (e.g., SIGINT) was received.
 *
 * This variable is set to 1 by signal handlers (set_flag and new_prompt)
 * when the shell is interrupted. It allows the shell to detect abrupt
 * termination and perform appropriate cleanup or refresh the interactive
 * prompt.
 *
 * @note	Declared volatile to prevent compiler optimizations and ensure
 *			the value is correctly read across signal handlers.
 * @note	Declared sig_atomic_t to guarantee atomic reads and writes in
 *			signal handlers.
 * @note	Initialized to 0 (no signal received).
 */
volatile sig_atomic_t	g_signal_received = 0;

/**
 * @brief Restores default handlers for SIGINT, SIGQUIT, and SIGPIPE.
 *
 * Sets the specified signals to their default behavior (SIG_DFL), which
 * is typically used before executing child processes to ensure that
 * signals are handled normally. This allows child processes to respond
 * to interrupts or quits as expected.
 *
 * @note	Does not modify g_signal_received; only affects signal handling.
 * @note	Useful for ensuring that child processes are not affected by
 *			custom shell signal handlers.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigdfl(void)
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
 * @brief Configures the shell to ignore SIGINT and SIGQUIT signals.
 *
 * Sets the shell to ignore SIGINT (Ctrl+C) and SIGQUIT (Ctrl+\) using
 * sigaction. The signal mask blocks SIGINT and SIGQUIT during handling,
 * and SA_RESTART is used to automatically restart interrupted system calls.
 *
 * @note	Specifically used before heredoc input to allow the user to
 *			enter data without generating new prompts or interruptions.
 * @note	Does not modify g_signal_received.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigign(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief Sets up a SIGINT handler for non-interactive shell mode.
 *
 * Configures the shell to handle SIGINT (usually Ctrl+C) by calling
 * set_flag, which sets g_signal_received to 1. Other parts of the shell
 * check this flag to detect abrupt termination and perform cleanup.
 *
 * @note	Blocks SIGQUIT and SIGINT during handling, and uses SA_RESTART
 *			to automatically restart interrupted system calls.
 * @note	Used only in non-interactive mode to ensure proper cleanup.
 * @note	set_flag does not perform cleanup itself; it only sets the flag.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int shell_signint(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = set_flag;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief Sets up a SIGINT handler for the interactive shell prompt.
 *
 * Configures the shell to handle SIGINT (usually Ctrl+C) by calling
 * new_prompt. This refreshes the prompt, prints a newline, clears the
 * current readline buffer, and forces a fresh prompt display.
 *
 * @note	Blocks SIGQUIT and SIGINT during handling, and uses SA_RESTART
 *			to automatically restart interrupted system calls.
 * @note	new_prompt sets g_signal_received to indicate an interrupt
 *			occurred, allowing other parts of the shell to respond if
 *			needed.
 * @note	Used only in interactive mode.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigint(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = new_prompt;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief Configures the shell to ignore the SIGQUIT signal.
 *
 * Sets SIGQUIT (usually triggered by Ctrl+\) to be ignored by the shell
 * process using sigaction. This ensures that the shell is not terminated
 * or dumped by SIGQUIT while running interactively or in non-interactive
 * mode.
 *
 * @note	Sets sa_handler = SIG_IGN and clears the signal mask.
 * @note	Does not affect child processes; they may still receive SIGQUIT
 *			unless modified separately.
 * @note	Does not modify g_signal_received because SIGQUIT is ignored.
 *
 * @return	MSHELL_SUCCESS on successful setup, MSHELL_FAILURE on error.
 */
int	shell_sigquit(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}
