/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/25 22:07:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

/**
 * Shell functions shouldn't need T_BODY, just as the libft, it should work with only
 * with necesary information. Insead of the T_BODY, you should ask for the T_LST!
 */
char	*shell_getenv(t_body *minishell, const char *name)
{
	t_env	*tmp;

	tmp = minishell->lst_env;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, (ft_strlen(name) + 1)))
		{
			if (!tmp->value[0])
				return (NULL);
			return (tmp->value);
		}
		tmp = tmp->current_next;
	}
	return (NULL);
}
