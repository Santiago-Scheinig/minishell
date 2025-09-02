/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/31 20:13:05 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"


static t_list *remove_lst(t_list *list)
{
	t_list	*current;
	t_var	*var;

	var = (t_var *)list->content;
	free(var->name);
	if (var->value)
		free(var->value);
	free(var);
	current = list->next;
	free(list);
	return (current);
}

char	**unset(char **envp, t_list *env_lst, char **name)
{
	t_list	*current;
	t_list	*prev;
	t_var	*var;
	size_t	i;

	current = env_lst;
	prev = NULL;
	while (current)
	{
		i = 0;
		var = (t_var *)current->content;
		while (name[i] && !ft_strncmp(var->name, name[i], ft_strlen(var->name) + 1))
		{
			if (prev)
				prev->next = current->next;
			else
				env_lst = current->next;
			envp = ft_remove_arr(envp, i);
			current = remove_lst(current);
			return (envp);
		}
		prev = current;
		current = current->next;
	}
	return (envp);
}
