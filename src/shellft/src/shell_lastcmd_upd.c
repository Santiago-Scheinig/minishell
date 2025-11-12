/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lastcmd_upd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 17:56:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:43:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Updates the special environment variable '_' with last cmd.
 *
 * 			Creates a string "_=cmd" and updates the linked list of
 *			environment variables and envp array. Adds it if it doesn't exist.
 *
 * @param	cmd		Last executed command string to store.
 * @param	envp	Double pointer to the environment variables array.
 * @param	head_envp	Linked list head of environment variables (t_var nodes).
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
int	shell_lastcmd_upd(char *cmd, char ***envp, t_list *head_envp)
{
	char	*var;
	int		export;

	export = 0;
	var = ft_strjoin("_=", cmd);
	if (!var)
		return (MSHELL_FAILURE);
	if (shell_envchr(&export, "_", head_envp))
	{
		if (shell_envlst_swp(export, var, envp, head_envp))
		{
			free(var);
			if (errno == ENOMEM)
				return (MSHELL_FAILURE);
		}
	}
	else if (shell_envlst_add(export, var, envp, head_envp))
	{
		free(var);
		if (errno == ENOMEM)
			return (MSHELL_FAILURE);
	}
	free(var);
	return (MSHELL_SUCCESS);
}
