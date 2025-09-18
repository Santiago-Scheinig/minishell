/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigend.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 14:54:17 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 19:45:03 by sscheini         ###   ########.fr       */
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

/**
 * COMMENT PENDING ISMA
 */
static void	new_prompt(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

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
int	sigquit(void)
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
