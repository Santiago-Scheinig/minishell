/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:56:42 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/08 18:39:30 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * Initializes the shell terminal and process group.
 * 
 * This function checks if STDIN is a terminal, sets the shell
 * to its own process group, assigns terminal control to it, and
 * configures terminal settings to show control characters (like ^C).
 * 
 * Steps performed:
 * 
 * - Checks if STDIN is a terminal (interactive shell).
 * - Sets the current process as the leader of its own process group.
 * - Takes control of the terminal with `tcsetpgrp`.
 * - Retrieves current terminal settings.
 * - Modifies terminal flags to enable ECHOCTL (shows ^C, ^\ on screen).
 * 
 * @note - ECHOCTL makes control characters like ^C visible in the prompt.
 * @note - If any system call fails, the function prints an error and exits.
 * 
 * @warning - Must be called once at shell startup before prompting the user.
 * 
 */

static void	config_shell(struct termios term, int shell_terminal)
{
	term.c_lflag |= ECHOCTL;
	if (tcsetattr(shell_terminal, TCSANOW, &term))
	{
		perror("Error setting TCSANOW term");
		exit (1);
	}
}
 
void	initialization(void)
{
	struct termios	term;
	pid_t			shell_pgid;
	int				shell_terminal;

	shell_terminal = STDIN_FILENO;
	if (!isatty(shell_terminal))
		return ;
	shell_pgid = getpid();
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		perror("Couldn't put the shell in tis own process group");
		exit(1);
	}
	tcsetpgrp(shell_terminal, shell_pgid);
	if (tcgetattr(shell_terminal, &term))
	{
		perror("Error setting STDIN_FILENO term");
		exit (1);
	}
	config_shell(term, shell_terminal);
	
}
