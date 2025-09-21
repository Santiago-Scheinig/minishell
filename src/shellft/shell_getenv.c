/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/21 17:01:38 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * COMMENT PENDING ISMA
 */
char	*shell_getenv(t_list *lst_var, const char *name)
{
	t_list	*tmp;
	t_var	*aux;
		
	tmp = lst_var;
	shell_sortenv(&tmp);
	while (tmp)
	{
		aux = (t_var *)tmp->content;
		if (!ft_strncmp(aux->name, name, (ft_strlen(name))))
		{
			if (!aux->value[0])
				return (NULL);
			return (aux->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
