/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 17:31:17 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

/*static invalid_flag(char **args)
{
	
}
*/
void	built_unset(char **args, t_env *env_lst)
{
	t_env	*current;
	t_env	*prev;
	int		len;

	//if (args[1])
	//	invalid_flag(args);
	current = env_lst;
	prev = NULL;
	while (current)
	{
		len = ft_strlen(current->name) + 1;
		if (!ft_strncmp(current->name, args[1], len))
		{
			if (prev)
				prev->current_next = current->current_next;
			else
				env_lst = current->current_next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->current_next;
	}
}
