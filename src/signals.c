/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:13:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/17 17:52:10 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * Global flag set when SIGINT (Ctrl+C) is received.
 * 
 * This flag is used to notify the shell that the signal occurred,
 * allowing proper prompt refresh and cleanup.
 * 
 * @note Declared volatile because it is modified from within a signal handler.
 */

volatile sig_atomic_t	g_signal_received = 0;

/**
 * Signal handler for SIGINT (Ctrl+C).
 * 
 * When triggered, this function sets a global flag indicating that the
 * signal was received, prints a newline, clears the current input line,
 * and redisplays the prompt using readline utilities.
 * 
 * @param signum The signal number received (unused).
 * 
 * @note Readline functions are used to ensure the shell prompt is restored
 *       correctly after the user presses Ctrl+C.
 * @note This handler only affects the shell prompt context.
 */

static void	ctrl_c(int signum)
{
	(void)signum;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * Sets up the signal handler for SIGINT (Ctrl+C).
 * 
 * This function installs a custom handler for SIGINT using sigaction,
 * and blocks both SIGINT and SIGQUIT during the handler's execution
 * to prevent nested signals from interfering.
 * 
 * @param minishell A pointer to the shell context, used for cleanup on failure
 * 
 * @return A pointer to the minishell context, or NULL on failure.
 * 
 * @note Uses SA_RESTART to automatically restart interrupted syscalls
 *       (like readline).
 * @note The signal handler function used is ctrl_c().
 */

static t_body	*sigint(t_body *minishell)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = ctrl_c;
	sigemptyset(&sa_int.sa_mask);
	sigaddset(&sa_int.sa_mask, SIGQUIT);
	sigaddset(&sa_int.sa_mask, SIGINT);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		cleanup(minishell);
		perror("Error setting SIGINT handler");
		return (NULL);
	}
	return (minishell);
}

/**
 * Ignores the SIGQUIT signal (Ctrl+\).
 * 
 * This function sets the SIGQUIT signal handler to SIG_IGN,
 * which tells the system to ignore it entirely.
 * 
 * @param minishell A pointer to the shell context, used for cleanup on failure
 * 
 * @return A pointer to the minishell context, or NULL on failure.
 * 
 * @note SIGQUIT is typically used to quit a process and produce a core dump.
 *       In an interactive shell, it is common to ignore it at the prompt.
 */

static t_body	*sigquit(t_body *minishell)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		cleanup(minishell);
		perror("Error setting SIGQUIT handler");
		return (NULL);
	}
	return (minishell);
}

/**
 * Sets up signal handlers for the shell prompt context.
 * 
 * Installs custom handler for SIGINT and ignores SIGQUIT.
 * Both are common signals triggered by user keyboard actions
 * (Ctrl+C and Ctrl+\ respectively).
 * 
 * @param minishell A pointer to the shell context.
 * 
 * @return A pointer to the minishell context, or NULL if setup failed.
 * 
 * @note This is meant to be called before prompting the user.
 */

static t_body	*handle_signals(t_body *minishell)
{
	if (!sigint(minishell))
		return (NULL);
	if (!sigquit(minishell))
		return (NULL);
	return (minishell);
}

/**
 * Handles shell input loop and signal setup.
 * 
 * This function installs signal handlers for SIGINT and SIGQUIT,
 * performs cleanup, and prompts the user using readline.
 * If the user inputs EOF (Ctrl+D), the shell exits.
 * Otherwise, the input is added to the readline history.
 * 
 * @param minishell A pointer to the shell context (includes state, input, etc.).
 * 
 * @note If readline returns NULL (Ctrl+D), the shell exits cleanly.
 * @note Cleanup is called before each input to reset shell state.
 */

//temporaly free lst
void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	recive_signals(t_body *minishell)
{
	if (!handle_signals(minishell))
		return ;
	cleanup(minishell);
	minishell->input = readline(minishell->prompt);
	if (minishell->input == NULL)
	{
		free_env_list(minishell->env);
		minishell->env = NULL;
		if (minishell->prompt)
		{
			free(minishell->prompt);
			minishell->prompt = NULL;
		}
		rl_clear_history();
		exit(0);
	}
	else if (minishell->input[0] != '\0')
		add_history(minishell->input);
}
