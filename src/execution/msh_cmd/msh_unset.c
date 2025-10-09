/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 05:53:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

static t_list	*remove_lst(t_list *lst_t_var)
{
	t_list	*aux_t_var;
	t_var	*var;

	var = (t_var *)lst_t_var->content;
	aux_t_var = lst_t_var;
	free(var->name);
	if (var->value)
		free(var->value);
	free(var);
	aux_t_var = lst_t_var->next;
	free(aux_t_var);
	return (lst_t_var);
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

static int	check_name(char *name, char ***envp, t_list **head_t_var)
{
	size_t	lst_len;
	size_t	i;
	size_t	j;
	t_var	*var;

	i = 0;
	var = (t_var *)(*head_t_var)->content;
	lst_len = ft_strlen(var->name);
	if (!ft_strncmp(var->name, name, lst_len))
	{
		j = 0;
		while (envp[0][j])
		{
			if (same_name_env(envp[0][j], name))
			{
				envp[0] = ft_remove_arr(envp[0], j);
				break ;
			}
			j++;
		}
		head_t_var[0] = remove_lst(head_t_var[0]);
		return (1);
	}
	return (0);
}

/**
 * Need fix
 */
int	msh_unset(char **arg, char ***envp, t_list **head_t_var)
{
	t_list	*lst_t_var;
	t_list	*prev_t_var;
	t_list	*next_t_var;
	int		i;

	i = -1;
	if (arg[0][0] == '-')
		return (built_end("unset", "Invalid flags", "[name ...]", arg[0][1]));
	while (arg[++i])
	{
		prev_t_var = NULL;
		lst_t_var = *head_t_var;
		while (lst_t_var && lst_t_var->content)
		{
			next_t_var = lst_t_var->next;
			if (check_name(arg[i], envp, &lst_t_var))
			{
				if (!prev_t_var)
					*head_t_var = lst_t_var;
				else
					prev_t_var->next = lst_t_var;
			}
			else
			{
				prev_t_var = lst_t_var;
				lst_t_var = next_t_var;
			}
		}
	}
	return (0);
}
