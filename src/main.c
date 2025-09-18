/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:50:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/24 15:50:09 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Global flag use to track signals.
 * 
 * This flag is used to notify the shell that the signal was intercepted,
 * allowing proper prompt refresh and cleanup.
 * 
 * @note Declared volatile because it is modified from within a signal handler.
 */
volatile sig_atomic_t	g_signal_received = 0;

// Creates an array of signals so i can then set it with 
// sigmask to ign on childs or non-interactive mode
// static int	sigset(void)

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
static int	sigint(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = new_prompt;
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
static int	sigquit(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		perror("Error setting SIGQUIT handler");
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}


static void	initialization(t_body *minishell, const char **envp)
{
	struct termios	new_term;

	ft_memset(minishell, 0, sizeof(t_body));
	minishell->interactive = isatty(STDIN_FILENO);
	if (minishell->interactive && isatty(STDOUT_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &(minishell->orig_term)))
			forcend(minishell, "tcgetattr", MSHELL_FATAL);
		new_term = minishell->orig_term;
		new_term.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term))
			forcend(minishell, "tcsetattr", MSHELL_FATAL);
		if (sigquit() || sigint())
			forcend(minishell, "sigaction", MSHELL_FAILURE);
	}
	else if (signal(SIGINT, SIG_IGN) || signal(SIGQUIT, SIG_IGN))
			forcend(minishell, "signal", MSHELL_FAILURE);
	minishell->envp = shell_envpdup(envp);
	if (!minishell->envp)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	minishell->envp_lst = shell_newlst_var(minishell->envp);
	if (!minishell->envp_lst)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	sortenv(&minishell->envp_lst);
}

int	main(int argc, char **argv, const char **envp)
{
	t_body	minishell;

	initialization(&minishell, envp);
	if (argc > 1 || argv[1])
		return (forcend(&minishell, argv[1], MSHELL_CMD_NOTEXE));
	while (1)
	{
		if (parser(&minishell))
			continue;
		// if (execmd(&minishell))
		// 	continue;
		//waitcmd(&minishell);
	}
	return (0);
}
