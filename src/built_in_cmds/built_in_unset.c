/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/23 17:08:13 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

void	built_unset(t_env *env_lst, const char *name)
{
	t_env	*current;
	t_env	*prev;
	int		len;

	current = env_lst;
	prev = NULL;
	while (current)
	{
		len = ft_strlen(current->name) + 1;
		if (!ft_strncmp(current->name, name, len))
		{
			if (prev)
				prev->current->next = current->current->next;
			else
				env_lst = current->current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->current->next;
	}
}
