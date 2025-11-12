/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:11:20 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/12 17:29:05 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Frees and removes a node from the environment list.
 *
 *			Frees the t_var struct inside 'lst_t_var' and its
 *			name/value strings. Returns the next node in the list.
 *
 * @param	lst_t_var	Node to remove.
 *
 * @return	Pointer to the next node, or NULL if none.
 */
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

/**
 * @brief	Checks if an environment string matches a variable name.
 *
 *			Compares characters up to '=' in 'env' and full 'name'.
 *			Returns 1 if they match, 0 otherwise.
 *
 * @param	env		Environment string (e.g., "VAR=value").
 * @param	name	Variable name to compare.
 *
 * @return	1 if 'env' matches 'name', 0 otherwise.
 */
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

/**
 * @brief	Searches for a variable and removes it if found.
 *
 *			Checks if the variable in '*lst_var' matches 'name'.
 *			If found, removes it from 'envp' and the linked list.
 *
 * @param	name	Variable name to remove.
 * @param	envp	Double pointer to environment array.
 * @param	lst_var	Double pointer to current node in linked list.
 *
 * @return	1 if a variable was removed, 0 otherwise.
 */
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

/**
 * @brief	Updates linked list pointers when a node is removed.
 *
 *			Sets the head pointer or the previous node's next pointer
 *			to 'lst_t_var' after a removal operation.
 *
 * @param	head_var	Double pointer to head of the list.
 * @param	prev_t_var	Previous node, or NULL if removing head.
 * @param	lst_t_var	Current node to attach.
 */
static void	move_lst(t_list **head_var, t_list *prev_t_var, t_list *lst_t_var)
{
	if (!prev_t_var)
		*head_var = lst_t_var;
	else
		prev_t_var->next = lst_t_var;
}

/**
 * @brief	Implements the 'unset' shell built-in command.
 *
 *			Removes variables from the environment array and the
 *			t_var linked list. Frees memory of removed variables.
 *
 *			Static helpers:
 *
 *				- remove_lst():	Frees a node from the linked list.
 *				- same_name_env():	Checks if env string matches a name.
 *				- check_name():		Removes a variable from envp/list.
 *				- move_lst():		Updates linked list pointers.
 *
 * @param	arg			Array of variable names to unset.
 * @param	envp		Double pointer to environment array.
 * @param	head_t_var	Double pointer to head of t_var linked list.
 *
 * @note	Invalid flags (starting with '-') trigger an error.
 * @note	Removes variables from both envp and the linked list.
 *
 * @return	0 on success, or error code on invalid input.
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
