/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   troubleshoot_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:59:51 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/25 22:19:38 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

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
//WHERES THE FUNCTION THAT HANDLES THIS??





//temporaly free lst
void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->current_next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}
