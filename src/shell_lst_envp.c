/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_lst_envp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:11:05 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/29 14:22:53 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_env	*create_envp(char *envp, int *error)
{
	t_env	*new;
	char	*sign;
	int		i;

	i = 0;
	error = 0;
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
