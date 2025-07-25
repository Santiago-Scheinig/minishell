/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 17:13:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/25 19:13:36 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

static void	print(int sign)
{
	pid_t pid;
    // Leer PIDs del pipe hasta que no queden
    while (read(pipe_fds[0], &pid, sizeof(pid)) == sizeof(pid))
    {
        kill(pid, SIGTERM); // o SIGKILL si quieres matar forzadamente
    }
	
}

void	recive_signals(t_body *minishell)
{
	char	*input;

	input = NULL;
	signal(SIGINT, print);
	minishell->errno = 0;
	input = readline("minishell> ");
}