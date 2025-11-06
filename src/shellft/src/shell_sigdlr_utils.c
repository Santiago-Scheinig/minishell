/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_sigdlr_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 21:53:23 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:46:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Sets the global signal variable to the received signal number.
 *
 *			Updates the g_signal_received variable with the value of signum,
 *			allowing the shell to know exactly which signal was caught.
 *
 * @param	signum	The signal number received.
 *
 * @note	Used in conjunction with signal handlers to track specific signals.
 */
void	sigdlr_setflg(int signum)
{
	g_signal_received = signum;
}

/**
 * @brief	Signal handler to refresh the shell prompt.
 *
 *			Sets the shell signal flag using sigdlr_setflg(), prints a newline,
 *			clears the current readline input, moves to a new line, and
 *			redisplays the prompt using the readline library functions.
 *
 * @param	signum	The signal number received (ignored in this function).
 *
 * @note	Designed to handle SIGINT (Ctrl+C) during prompt input.
 * @note	Does not terminate the shell; only resets the input line.
 */
void	sigdlr_newpmt(int signum)
{
	sigdlr_setflg(signum);
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
