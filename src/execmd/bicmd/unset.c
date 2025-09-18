/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/17 15:15:47 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

static t_list	*remove_lst(t_list *list)
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

static int	same_name_env(const char *env, const char *name)
{
	size_t	i;

	i = 0;
	while (env[i] && env[i] != '=' && name[i])
	{
		if (env[i] != name[i])
			return (0);
		i++;
	}
	if (env[i] == '=' && name[i] == '\0')
		return (1);
	return (0);
}

static int	check_name(char **envp, char *name, t_list **lst)
{
	size_t	lst_len;
	size_t	i;
	size_t	j;
	t_var	*tmp;

	i = 0;
	tmp = (t_var *)(*lst)->content;
	lst_len = ft_strlen(tmp->name);
	if (!ft_strncmp(tmp->name, name, lst_len) && !tmp->name[lst_len])
	{
		j = 0;
		while (envp[j])
		{
			if (same_name_env(envp[j], name))
			{
				envp = ft_remove_arr(envp, j);
				break ;
			}
			j++;
		}
		lst[0] = remove_lst(lst[0]);
		return (1);
	}
	return (0);
}

char	**unset(char **envp, t_list *env_lst, char **arg)
{
	t_list	**current;
	size_t	i;

	i = 0;
	if (arg[0][0] == '-')
	{
		built_end("unset", "Invalid flags", "[name ...]", arg[0][1]);
		return (NULL);
	}
	while (arg && arg[i])
	{
		current = &env_lst;
		while (*current)
		{
			if (check_name(&envp[0], arg[i], current))
				continue ;
			current = &((*current)->next);
		}
		i++;
	}
	return (envp);
}
