/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_signal_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:53:23 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 21:53:48 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Sets the global signal flag to indicate an interrupt.
 *
 * Signal handler that sets the g_signal_received flag to 1 when a
 * SIGINT (or other registered signal) is received. Used by the shell
 * to detect abrupt termination and trigger cleanup in non-interactive
 * mode.
 *
 * @param signum The received signal number (ignored in this function).
 *
 * @note	Only sets the global flag; does not perform cleanup itself.
 */
void	set_flag(int signum)
{
	(void)signum;
	g_signal_received = 1;
}

/**
 * @brief Signal handler to refresh the interactive prompt after SIGINT.
 *
 * Sets the global signal flag using set_flag, prints a newline to move
 * the cursor to a new line, clears the current readline buffer, and
 * forces readline to display a fresh prompt.
 *
 * @param signum The received signal number (typically SIGINT).
 *
 * @note	For interactive prompt context only.
 * @note	Uses async-signal-safe functions where applicable.
 * @note	Sets g_signal_received to allow other parts of the shell to
 *			react to the interrupt if necessary.
 */
void	new_prompt(int signum)
{
	(void)signum;
	set_flag(signum);
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}