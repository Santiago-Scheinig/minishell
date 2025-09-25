/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/25 16:28:12 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

static t_list	*remove_lst(t_list *list)
{
	t_list	*current;
	t_var	*var;

	var = (t_var *)list->content;
	current = list;
	free(var->name);
	if (var->value)
		free(var->value);
	free(var);
	list = current->next;
	free(current);
	return (list);
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

static int	check_name(char ***envp, char *name, t_list **lst)
{
	size_t	lst_len;
	size_t	i;
	size_t	j;
	t_var	*tmp;

	i = 0;
	tmp = (t_var *)(*lst)->content;
	lst_len = ft_strlen(tmp->name);
	if (!ft_strncmp(tmp->name, name, lst_len))
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
		lst[0] = remove_lst(lst[0]);
		return (1);
	}
	return (0);
}

/**
 * Removes an element from a NULL-terminated array of strings.
 * 
 * @param arr Array of strings to modify.
 * @param index Index of the element to remove.
 * 
 * Frees the removed element and shifts remaining pointers down; sets final
 * slot to NULL.
 * 
 * @return The modified array pointer.
 */
char	**ft_remove_arr(char **arr, int index)
{
	int		len;

	if (!arr)
		return (arr);
	len = ft_arrlen((const void **)arr);
	if (index >= len)
		return (arr);
	free(arr[index]);
	while (index < len - 1)
	{
		if (arr[index + 1] != NULL)
			arr[index] = arr[index + 1];
		else
			arr[index] = NULL;
		index++;
	}
	arr[len - 1] = NULL;
	return (arr);
}

/**
 * Built-in 'unset' command implementation for msh.
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param env_lst Pointer-to-list head of environment variables.
 * @param arg Array of variable names to unset.
 * 
 * Validates flags, then removes each matching variable from both envp and the
 * list.
 * 
 * @return 0 on success, non-zero on error.
 */
int msh_unset(char ***envp, t_list **env_lst, char **arg)
{
	t_list	*current;
	t_list	*prev;
	t_list	*next;
	int		i;

	i = -1;
	if (arg[0][0] == '-')
		return (built_end("unset", "Invalid flags", "[name ...]", arg[0][1]));
	while (arg[++i])
	{
		prev = NULL;
		current = *env_lst;
		while (current && current->content)
		{
			next = current->next;
			if (check_name(envp, arg[i], &current))
			{
				if (!prev)
					*env_lst = current;
				else
					prev->next = current;
			}
			else
			{
				prev = current;
				current = next;
			}
		}
	}
	return (0);
}
