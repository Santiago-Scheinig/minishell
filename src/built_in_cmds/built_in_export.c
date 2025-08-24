/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 16:57:22 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static void	print_end(char *name, char *type)
{
	char	*shell;

	shell = "minishell: ";
	if (ft_strnstr(type, "Not valid identifier", ft_strlen(type)))
		ft_printfd(2, "%s%s: `%s': not a valid identifier\n", shell,
		name, strerror(errno));
}

static int	ft_isal_num(char **new_e, t_env *head)
{
	int	error;
	int	i;
	int	j;

	i = 0;
	if (!new_e[1])
		print_export(head);
	if (new_e[1][0] == '-' && new_e[i + 1])
		built_end("export", "Invalid flags", "name[=value ...]", new_e[1][i + 1]);
	error = 0;
	j = 0;
	while (new_e[++j])
	{
		while (new_e[j][i])
		{
			if (!ft_isalnum(new_e[j][i]) || new_e[j][i] != '_')
				break;
			i++;
		}
		if (!new_e[j][i] || new_e[j][i] != '=')
			print_end("export", "Not valid identifier");
	}
	if (error)
		built_end("export", NULL, NULL, '\0');
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
		built_end("export", "malloc", NULL, '\0');
	current = current->current_next;
	return (current);
}

static t_env *new_envp(char **new_env, t_env *head)
{
	t_env	*new_node;
	t_env	*current;
	int		error;
	int		i;

	i = 0;
	while (new_env[i])
	{
		new_node = create_envp(new_env[i], &error);
		if (!new_node)
			return (NULL);
		current = head;
		while (current->current_next)
		current = current->current_next;
		current->current_next = new_node;
	}
	return (head);
}

t_env	*built_export(char **envp, t_env *head, char **new_env)
{
	t_env	*crnt;
	int		i;
	int		j;

	crnt = head;
	j = 0;
	ft_isal_num(new_env, head);
	while (crnt)
	{
		i = 0;
		while (new_env[++i])
			if (!ft_strncmp(crnt->name, new_env[i], ft_strlen(crnt->name) + 1))
				change_value_env(crnt, new_env[i]);
		crnt = crnt->current_next;
		j++;
	}
	ft_realloc(envp, new_env, j + i);
	new_envp(new_env, head);
	return (head);
}
