/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/23 14:10:45 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

static t_env	*copy_envp(t_env *new, char *envp, char *sign, int i)
{
	char	*tmp;

	tmp = malloc((i + 1) * sizeof(char));
	if (!tmp)
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(tmp, envp, i);
	tmp[i] = '\0';
	new->name = ft_strdup(tmp);
	free(tmp);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (!sign)
		return (new);
	new->value = ft_strdup(sign + 1);
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	return (new);
}

static t_env	*create_envp(char *envp, int *error)
{
	t_env	*new;
	char	*sign;
	int		i;

	i = 0;
	sign = ft_strchr(envp, '=');
	new = malloc(sizeof(t_env));
	if (!new)
	{
		error[0] = 1;
		return (NULL);
	}
	while (envp[i] && envp[i] != '=')
		i++;
	new = copy_envp(new, envp, sign, i);
	if (!new)
	{
		error[0] = 1;
		return (NULL);
	}
	new->exported = 1;
	new->current_next = NULL;
	return (new);
}

t_env	*init_envp(char **envp)
{
	t_env	*new_node;
	t_env	*current;
	t_env	*head;
	int		error;
	int		i;

	new_node = NULL;
	current = NULL;
	head = NULL;
	error = 0;
	i = -1;
	while (envp[++i])
	{
		new_node = create_envp(envp[i], &error);
		if (error)
			return (NULL);
		if (!new_node)
			continue ;
		if (!head)
			head = new_node;
		else
			current->current_next = new_node;
		current = new_node;
	}
	return (head);
}

static t_env	*change_value_env(t_env *current, char *new_env)
{
	char	*sign;

	sign = ft_strchr(new_env, '=');
	if (!sign)
		return (NULL);
	free(current->value);
	current->value = ft_strdup(sign + 1);
	if (!current->value)
		return (NULL);
	return (current);
}

t_env	*add_env(t_env *head, char *new_env)
{
	t_env	*new_node;
	t_env	*current;
	int		error;

	error = 0;
	current = head;
	while (current)
	{
		if (!ft_strncmp(current->name, new_env, ft_strlen(current->name) + 1))
		{
			if (!change_value_env(current, new_env))
				return (NULL);
			return (head);
		}
		current = current->current_next;
	}
	new_node = create_envp(new_env, &error);
	if (!new_node)
		return (NULL);
	current = head;
	while (current->current_next)
		current = current->current_next;
	current->current_next = new_node;
	return (head);
}
