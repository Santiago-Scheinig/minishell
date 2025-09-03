/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_newlst_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:48:40 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/03 21:57:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * COMMENT PENDING
 */
static t_var	*copy(t_var *new, const char *envp, char *sign, int i)
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
	{
		tmp[i - 1] = '\0';
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
 * COMMENT PENDING
 */
t_var	*create_envp(const char *envp)
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
	new = copy(new, envp, sign, i + 1);
	if (!new)
		return (NULL);
	new->exported = 1;
	return (new);
}

/**
 * COMMENT PENDING
 */
t_list	*shell_newlst_var(char **envp)
{
	t_var	*content;
	t_list	*new_node;
	t_list	*head;
	int			i;

	new_node = NULL;
	head = NULL;
	i = -1;
	while (envp[++i])
	{
		content = create_envp(envp[i]);
		if (!content)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return (NULL);
		}
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			shell_lstclear(&head, shell_lstdelvar);
			return(NULL);
		}
		ft_lstadd_back(&head, new_node);
	}
	return (head);
}
