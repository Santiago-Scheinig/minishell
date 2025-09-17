/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unexport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:12 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/17 14:58:50 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

t_list	*inport(char ***envp, t_list *head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;
	int		j;

	tmp = head;
	j = 0;
	args = ft_isal_num(args, head);
	if (!args)
		return (head);
	while (tmp)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (!change_value_env(aux, &envp[0], args[i]))
					args = ft_remove_arr(&args[0], i);
		tmp = tmp->next;
	}
	if (!new_envp(args, head, 0))
		return (NULL);
	return (head);
}