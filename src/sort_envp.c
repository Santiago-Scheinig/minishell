/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/17 19:41:37 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

void	print_env(t_env *env_lst)
{
	t_env	*current;

	current = env_lst;
	while (current)
	{
		if (current->name && current->exported)
		{
			printf("%s", current->name);
			if (current->value)
				printf("=\"%s\"", current->value);
			printf("\n");
		}
		current = current->next;
	}
}

void	print_export(t_env *env_lst)
{
	t_env	*current;

	current = env_lst;
	while (current)
	{
		if (current->name && current->exported)
		{
			printf("declare -x %s", current->name);
			if (current->value)
				printf("=\"%s\"", current->value);
			printf("\n");
		}
		current = current->next;
	}
}

static void	swap_env(t_env *a, t_env *b)
{
	char	*tmp_name;
	char	*tmp_value;
	int		tmp_exported;

	tmp_name = a->name;
	tmp_value = a->value;
	tmp_exported = a->exported;
	a->name = b->name;
	a->value = b->value;
	a->exported = b->exported;
	b->name = tmp_name;
	b->value = tmp_value;
	b->exported = tmp_exported;
}

void	sort_env(t_env *head)
{
	t_env	*current;
	int		sorted;
	int		len;

	sorted = 0;
	while (!sorted)
	{
		sorted = 1;
		current = head;
		while (current->next)
		{
			len = ft_strlen(current->name) + 1;
			if (ft_strncmp(current->name, current->next->name, len) > 0)
			{
				swap_env(current, current->next);
				sorted = 0;
			}
			current = current->next;
		}
	}
}
