/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lastcmd_upd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 17:56:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/05 16:38:05 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

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
