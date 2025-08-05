/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:56:42 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/05 17:35:08 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "signals.h"

void	initialization(void)
{
	pid_t	shell_pgisd;
	int		shell_terminal;

	shell_terminal = STDIN_FILENO;
	if (!isatty(shell_terminal))
		return ;
	shell_pgisd = getpid();
	if (setpgid(shell_pgid, shell_pgid) < 0)
	{
		perror("Couldn't put the shell  in tis own process group");
		exit(1);
	}
	tcsetpgrp(shell_terminal, shell_pgid);
}