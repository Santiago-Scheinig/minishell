/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/14 14:03:11 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*shell_getenv(t_list *lst_var, const char *name)
{
	t_list	*tmp;
	t_var	*aux;

	tmp = lst_var;
	sortenv(&tmp);
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
