/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:45:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

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

void	print_env(char **envp, t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;
	int		i;

	i = 0;
	while (envp[i])
	{
		current = env_lst;
		while (current)
		{
			tmp = (t_var *)current->content;
			if (!ft_strncmp(tmp->name, envp[i], ft_strlen(tmp->name)))
			{
				if (tmp->exported)
					ft_printf("%s\n", envp[i]);
				break ;
			}
			current = current->next;
		}
		i++;
	}
}

int	msh_env(char **args, char **envp, t_list *env_lst)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	else if (args[1])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	print_env(envp, env_lst);
	return (0);
}
