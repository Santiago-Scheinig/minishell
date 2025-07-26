/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:13:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/26 15:42:41 by ischeini         ###   ########.fr       */
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

static void	child_clean(t_body *minishell)
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
}

static void	ctrl_c(int signum)
{
	signal_received = 1;
}

static void	ctrl_quit(int signum)
{
	signal_received = 0;
}

static void	*handle_signals(t_body *minishell)
{
	if (signal(SIGINT, ctrl_c) == SIG_ERR)
	{
		cleanup();
		perror("Error setting SIGINT handler");
		return (NULL);
	}
	if (signal(SIGQUIT, ctrl_quit) == SIG_ERR)
	{
		cleanup();
		perror("Error setting SIGQUIT handler");
		return (NULL);
	}
}

void	recive_signals(t_body *minishell)
{
	int		bytes_read;
	int		i;
	
	if (!handle_signals(minishell))
		return ;
	if (signal_received)
	{
		child_clean(minishell);
		cleanup(minishell);
	}
	minishell->input = readline("minishell> ");
	if (minishell.input == NULL)
	{
		cleanup(minishell);
		exit(1);
	}
	else
		add_history(minishell->input);
}
