/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:13:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/05 14:24:45 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

/**
 * recive_signals - Handles signals for the minishell.
 * 
 * @param minishell The minishell structure containing the input and th childs.
 *  
 * Handles the SIGINT signal to allow the user to interrupt the shell.
 * It sets up a signal handler for SIGINT, which allows the user to interrupt
 * the shell with Ctrl-C. When the signal is received, it sets a flag
 * `signal_received` to indicate that a signal was received.
 *
 * This function reads input from the user and adds it to the history.
 * If the input is empty, it will not add it to the history.
 * It also handles the case where the user inputs "exit" to terminate the shell.
 *
 * @note - The function uses a global variable `signal_received` 
 * 		   to track if a signal was received.
 */

volatile sig_atomic_t	g_signal_received = 0;

/*static void	child_clean(t_body *minishell)
{
	int	i;

	i = 0;
	while (i < minishell->pipe_child_count)
	{
		if (minishell->pipe_child[i] > 0)
		{
			kill(minishell->pipe_child[i], SIGTERM);
			waitpid(minishell->pipe_child[i], NULL, 0);
		}
		i++;
	}
}*/

static void	ctrl_c(int signum)
{
	signum = 0;
	g_signal_received = 1;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	ctrl_do_nothing(int signum)
{
	rl_on_new_line();
	rl_redisplay();
	signum = 0;
}

static t_body	*handle_signals(t_body *minishell)
{
	if (signal(SIGINT, ctrl_c) == SIG_ERR)
	{
		cleanup(minishell);
		perror("Error setting SIGINT handler");
		return (NULL);
	}
	if (signal(SIGQUIT, ctrl_do_nothing) == SIG_ERR)
	{
		cleanup(minishell);
		perror("Error setting SIGQUIT handler");
		return (NULL);
	}
	return (minishell);
}

void	recive_signals(t_body *minishell)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term))
	{
		cleanup(minishell);
		perror("Error setting STDIN_FILENO term");
	}
	term.c_lflag |= ECHOCTL;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term))
	{
		cleanup(minishell);
		perror("Error setting TCSANOW term");
	}
	if (!handle_signals(minishell))
		return ;
	cleanup(minishell);
	minishell->input = readline("minishell> ");
	if (minishell->input == NULL)
	{	
		cleanup(minishell);
		exit(1);
	}
	else
		add_history(minishell->input);
}
