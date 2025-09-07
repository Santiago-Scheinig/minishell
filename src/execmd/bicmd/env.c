/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/07 16:12:46 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

void	print_export(t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;

	current = env_lst;
	while (current)
	{
		tmp = (t_var *)current->content;
		if (tmp->name && tmp->exported)
		{
			ft_printf("declare -x %s", tmp->name);
			if (tmp->value)
				ft_printf("=\"%s\"", tmp->value);
			ft_printf("\n");
		}
		current = current->next;
	}
}

void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		ft_printf("%s\n", envp[i]);
		i++;
	}
}

int	env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	else if (args[1])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	print_env(envp);
	return (0);
}
