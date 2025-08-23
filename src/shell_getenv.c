/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/23 13:00:20 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

char	*shell_getenv(t_body *minishell, const char *name)
{
	t_env	*tmp;

	tmp = minishell->env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, (ft_strlen(name) + 1)))
		{
			if (!tmp->value[0])
				return (NULL);
			return (tmp->value);
		}
		tmp = tmp->current->next;
	}
	return (NULL);
}
