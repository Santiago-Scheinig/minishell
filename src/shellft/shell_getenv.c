/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/03 21:56:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * COMMENT PENDING
 */
char	*shell_getenv(t_list *lst_var, const char *name)
{
	t_list	*tmp;
	t_var	*aux;

	tmp = lst_var;
	while (tmp)
	{
		aux = (t_var *)tmp->content;
		if (!ft_strncmp(aux->name, name, (ft_strlen(name) + 1)))
		{
			if (!aux->value[0])
				return (NULL);
			return (aux->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}
