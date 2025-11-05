/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/05 17:06:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

static t_list	*remove_lst(t_list *lst_t_var)
{
	t_list	*next_node;
	t_var	*var;

	if (!lst_t_var)
		return (NULL);
	var = (t_var *)lst_t_var->content;
	next_node = lst_t_var->next;
	free(var->name);
	if (var->value)
		free(var->value);
	free(var);
	free(lst_t_var);
	return (next_node);
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

static int	check_name(char *name, char ***envp, t_list **lst_var)
{
	size_t	lst_len;
	size_t	i;
	size_t	j;
	t_var	*var;

	i = 0;
	var = (t_var *)(*lst_var)->content;
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
		(*lst_var) = remove_lst((*lst_var));
		return (1);
	}
	return (0);
}

static void	move_lst(t_list **head_var, t_list *prev_t_var, t_list *lst_t_var)
{
	if (!prev_t_var)
		*head_var = lst_t_var;
	else
		prev_t_var->next = lst_t_var;
}

/**
 * @brief	Implements the 'unset' built-in command.
 *
 *			Removes one or more variables from the shell's environment
 *			and from the linked list of stored variables.
 *
 *			Static helpers:
 *
 *				- remove_lst():		Frees and removes a node from the
 *									variable list.
 *
 *				- same_name_env():	Checks if an environment string
 *									matches a given variable name.
 *
 *				- check_name():		Searches and removes a matching
 *									variable from both the environment
 *									array and the linked list.
 *
 * @param	arg			Array of variable names to unset. Flags
 *						(e.g., starting with '-') are rejected.
 * @param	envp		Double pointer to the environment array.
 * @param	head_t_var	Double pointer to the head of the t_var
 *						linked list storing environment variables.
 *
 * @note	If a variable exists in both envp and the linked list,
 *			it will be removed from both. The memory of its name and
 *			value is freed.
 * @note	Invalid flags trigger an error through shell_builterr().
 *
 * @return	0 on success, or an error code if input validation or
 *			memory operations fail.
 */
int	bin_unset(char **arg, char ***envp, t_list **head_t_var)
{
	t_list	*lst_t_var;
	t_list	*prev_t_var;
	t_list	*next_t_var;
	int		i;

	i = -1;
	if (arg[0][0] == '-')
		return (shell_binerr(INVFLGS, "unset", "name ...", arg[0][1]));
	while (arg[++i])
	{
		prev_t_var = NULL;
		lst_t_var = *head_t_var;
		while (lst_t_var && lst_t_var->content)
		{
			next_t_var = lst_t_var->next;
			if (check_name(arg[i], envp, &lst_t_var))
				move_lst(head_t_var, prev_t_var, lst_t_var);
			else
				prev_t_var = lst_t_var;
			lst_t_var = next_t_var;
		}
	}
	return (0);
}
