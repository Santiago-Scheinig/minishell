/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitcmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:11:35 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/03 21:59:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "troubleshoot.h"
/**
 * Waits for every child process executed to finish before exiting the
 * pipex program.
 * @param env The main enviroment pipex structure.
 * @param exit_status The status which the pipex program will exit.
 * @return If exit_error is false returns 0; else will return -1;
 * @note If any waitpid fails to execute, the program will exit by
 * force and non waited childs will remain as zombie.
 */
int	waitcmd(t_pipex *env, int exit_error)//error to waitpid (*int)
{
	int	i;

	i = -1;
	while (++i < env->cmd_count)
	{
		if (env->waitpid_list[i] > 0)
		{
			if (waitpid(env->waitpid_list[i], NULL, 0) == -1)
			{
				free(env->waitpid_list);
				return (ft_forcend(env, NULL, "Waitpid"));
			}
		}
	}
	free(env->waitpid_list);
	if (exit_error == EXIT_FAILURE)
		return (ft_forcend(env, NULL, "Pipe"));
	return (ft_forcend(env, NULL, NULL));
}