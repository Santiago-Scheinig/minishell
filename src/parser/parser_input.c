/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:13:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/27 01:12:09 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "bicmd.h"

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
 * Signal handler specifically for SIGINT.
 * 
 * When called, this function sets a global flag indicating that the
 * signal was received, prints a newline, clears the current input line,
 * and displays a new prompt using readline utilities.
 * 
 * @param signum The signal number received (unused).
 * @note Readline functions are used to ensure the shell prompt is restored
 * correctly.
 */

static void	sigend(int signum)
{
	(void)signum;
}

static void	new_prompt(int signum)
{
	(void)signum;
	g_signal_received = 1;//We are not resetting signal_received to 0 after all the promt is clear, shouldnt we? Otherwise, why are we even using it?
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static t_body	*siguser(t_body *minishell)
{
		struct sigaction	sa_user;

	sa_user.sa_handler = sigend;
	sigemptyset(&sa_user.sa_mask);
	sigaddset(&sa_user.sa_mask, SIGQUIT);
	sigaddset(&sa_user.sa_mask, SIGINT);
	sa_user.sa_flags = SA_RESTART;//This is blank and im not sure why...
	if (sigaction(SIGUSR1, &sa_user, NULL) == -1)//This intercepts a signal, it doesnt set or install. It tells the "system" how to behaive when this signal is intercepted.
	{
		cleanup(minishell);
		perror("Error setting SIGINT handler");//We are not setting anything, we are intercepting!
		return (NULL);
	}
	return (minishell);
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
static t_body	*sigint(t_body *minishell)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = new_prompt;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;//This is blank and im not sure why...
	if (sigaction(SIGINT, &sa_int, NULL) == -1)//This intercepts a signal, it doesnt set or install. It tells the "system" how to behaive when this signal is intercepted.
	{
		cleanup(minishell);
		perror("Error setting SIGINT handler");//We are not setting anything, we are intercepting!
		return (NULL);
	}
	return (minishell);
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

static t_body	*sigquit(t_body *minishell)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)//This intercepts a signal, it doesnt set or install. It tells the "system" how to behaive when this signal is intercepted.
	{
		cleanup(minishell);
		perror("Error setting SIGQUIT handler");//We are not setting anything, we are intercepting!
		return (NULL);
	}
	return (minishell);
}

/**
 * Intercepts certain user signals and handles them as the minishell 
 * prompt context indicates.
 * 
 * - SIGINT: User signal made with input [Ctrl+C].
 * - SIGQUIT: User signal made with input [Ctrl+\].
 * 
 * @param minishell A pointer to the main enviroment structure of minishell.
 * @return A pointer to the main enviroment structure of minishell, 
 * or NULL if the interception failed.
 */
static void	*handle_signals(t_body *minishell)
{
	if (!sigint(minishell))
		return (NULL);//This should be FORCEND (cuz we cant fail to intercept a signal)
	if (!sigquit(minishell))
		return (NULL);//This should be FORCEND (cuz we cant fail to intercept a signal)
	if (!siguser(minishell))
		return (NULL);
	return (minishell);
}

void	parser_input(t_body *minishell)
{
	if (!handle_signals(minishell))//removing the if here, cuz the return doesnt make sense!
		return ;
	sortenv(minishell->lst_env);
	cleanup(minishell);
	minishell->input = readline(minishell->prompt);//So, does readline has its own signal handler then? Cuz i couldnt find the function that handles CTRL+D, only its comment...
	if (minishell->input == NULL)
	{
		free_env_list(minishell->lst_export);
		free_env_list(minishell->lst_env);
		if (minishell->prompt)
			free(minishell->prompt);
		rl_clear_history();
		exit(0);
	}
	else if (!minishell->input[0])
		parser_input(minishell);
	else if (minishell->input[0] != '\0')
		add_history(minishell->input);
}
