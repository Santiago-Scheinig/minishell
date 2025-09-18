/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newlst_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:48:40 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 20:27:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * COMMENT PENDING ISMA
 */
static t_var	*init_envp(t_var *new, const char *envp, char *sign, int i)
{
	new->name = ft_substr(envp, 0, i);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	if (!sign)
	{
		new->value = NULL;
		return (new);
	}
	new->value = ft_strdup(sign + 1);
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	return (new);
}

/**
 * COMMENT PENDING ISMA
 */
t_var	*create_envp(const char *envp, int export)
{
	t_var	*new;
	char	*sign;
	int		i;

	i = 0;
	sign = ft_strchr(envp, '=');
	new = malloc(sizeof(t_var));
	if (!new)
		return (NULL);
	while (envp[i] && envp[i] != '=')
		i++;
	new = init_envp(new, envp, sign, i);
	if (!new)
		return (NULL);
	new->exported = export;
	return (new);
}

/**
 * COMMENT PENDING ISMA
 */
t_list	*shell_newlst_var(char **envp)
{
	t_var	*content;
	t_list	*new_node;
	t_list	*head;
	int		i;

	new_node = NULL;
	head = NULL;
	i = -1;
	while (envp[++i])
	{
		content = create_envp(envp[i], 1);
		if (!content)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return (NULL);
		}
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return (NULL);
		}
		ft_lstadd_back(&head, new_node);
	}
	return (head);
}
