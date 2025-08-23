/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/23 19:24:45 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static int	ft_isal_num(char *new_env)
{
	int	i;

	i = 0;
	while (new_env[++i])
		if (!ft_isalnum(new_env[i]) || new_env[i] != '_')
			break;
	if (!new_env[i] || new_env[i] != '=')
		return (0);
	return (1);
}

static t_env	*change_value_env(t_env *current, char *new_env)
{
	char	*sign;

	free(current->value);
	sign = ft_strchr(new_env, '=');
	if (!sign)
		return (NULL);
	current->value = ft_strdup(sign + 1);
	if (!current->value)
		return (NULL);
	current = current->current->next;
	return (current);
}

//ft_realloc
t_env	*built_export(char **envp, t_env *head, char *new_env)
{
	t_env	*new_node;
	t_env	*current;
	int		error;

	error = 0;
	current = head;
	while (error++ && current)
	{
		if (!ft_strncmp(current->name, new_env, ft_strlen(current->name) + 1))
		{
			if (!change_value_env(current, new_env))
				return (NULL);
			return (head);
		}
		current = current->current_next;
	}
	ft_realloc(envp, new_env, error + 2);
	new_node = create_envp(new_env, &error);
	if (!new_node)
		return (NULL);
	current = head;
	while (current->current_next)
		current = current->current_next;
	current->current_next = new_node;
	return (head);
}
