/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitcmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:11:35 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/24 19:12:00 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * 
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int	waitcmd(t_body *minishell)
{
	int	i;
	int	cmd_len;

	i = -1;
	cmd_len = ft_lstsize(minishell->cmd_lst);
	while (++i < cmd_len)
	{
		if (minishell->childs_pid[i] > 0)
		{
			waitpid(minishell->childs_pid[i], NULL, 0);
			if (minishell->interactive)
				sigint();
			close(minishell->err_fd);
			// {
				//print error based on &status like waitpid(minishell->childs_pid[i], &status, 0)
			// }
		}
	}
	return (MSHELL_SUCCESS);
}
