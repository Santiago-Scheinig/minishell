/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 19:34:08 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

static int	is_valid_identifier(char *arg)
{
	int i = 0;
	
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
	{
		ft_printfd(2, "minishell: export: `%s': not a valid identifier\n",
		arg);
		return (0);
	}
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			ft_printfd(2, "minishell: export: `%s': not a valid identifier\n",
			arg);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	ft_isal_num(char **new_e, t_env *head)
{
	int	error;
	int	i;
	int	j;

	error = 0;
	i = 0;
	if (!new_e[1])
	{
		print_export(head);
		return (2);	
	}
	if (new_e[1][0] == '-' && new_e[1][1])
		built_end("export", "Invalid flags", "name[=value ...]", new_e[1][1]);
	j = 0;
	while (new_e[++j])
	{
		if (!is_valid_identifier(new_e[j]) && new_e[j][i] != '_')
			error = 1;	
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

	i = -1;
	error = 0;
	while (new_env[++i])
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
	if (ft_isal_num(new_env, head) == 2)
		return (head);
	while (crnt)
	{
		i = 1;
		while (new_env[++i])
			if (!ft_strncmp(crnt->name, new_env[i], ft_strlen(crnt->name) + 1))
				change_value_env(crnt, new_env[i]);
		crnt = crnt->current_next;
		j++;
	}
	envp = ft_realloc(envp, &new_env[1], j + i - 1);
	new_envp(&new_env[1], head);
	return (head);
}
