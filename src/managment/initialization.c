/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:56:42 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/26 18:41:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		exit (1);//FORCEND or a return to actually do it in the INITIALIZATION.
	}
}

/**
 * This is initialization, i agree all of this to be a setup, but lets not confuse
 * signal handling with minishell setup.
 */
void	initialization(t_body *minishell, const char **envp)
{
	struct termios	term;
	pid_t			shell_pgid;
	int				shell_terminal;

	shell_terminal = STDIN_FILENO;
	if (!isatty(shell_terminal))
		return ;//FORCEND
	shell_pgid = getpid();
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		perror("Couldn't put the shell in tis own process group");
		exit(1);//FORCEND - If we forcend, theres no need to use the perror() above, cuz it's made inside of FORCEND passing it the correct errno that should print bash in case minishell ends like this.
	}
	tcsetpgrp(shell_terminal, shell_pgid);
	if (tcgetattr(shell_terminal, &term))
	{
		perror("Error setting STDIN_FILENO term");
		exit(1);//FORCEND - If we forcend, theres no need to use the perror() above, cuz it's made inside of FORCEND passing it the correct errno that should print bash in case minishell ends like this.
	}
	config_shell(term, shell_terminal);
	ft_memset(minishell, 0, sizeof(t_body));
	if (!shell_prompt(minishell))
		exit(1);//FORCEND
	minishell->lst_env = init_envp(envp);
	minishell->lst_export = init_envp(envp);
}
