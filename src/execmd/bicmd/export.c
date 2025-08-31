/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/31 20:12:47 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

static int	is_valid_identifier(char *arg)
{
	int i = 0;
	
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (built_end("export", "Not valid identifier", arg, '\0'));
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (built_end("export", "Not valid identifier", arg, '\0'));
		i++;
	}
	return (1);
}

static int	ft_isal_num(char **args, t_list *head)
{
	int	i;
	int	j;

	i = 0;
	if (!args[0])
	{
		print_export(head);
		return (2);	
	}
	if (args[0][0] == '-' && args[0][1])
		return (built_end("export", "Invalid flags", "name[=value ...]",
		args[0][1]));
	j = -1;
	while (args[++j])
	{
		if (!is_valid_identifier(args[j]) && args[j][i] != '_')
			return (1);
	}
	return (0);
}

static int	change_value_env(t_var *aux, char **envp, char *new_env)
{
	char	*sign;
	size_t	i;

	sign = ft_strchr(new_env, '=');
	i = 0;
	while (new_env[i] && new_env[i] != '=')
		i++;
	if (sign && i == ft_strlen(aux->name))
	{
		if (aux->value)
			free(aux->value);
		free(envp[0]);
		aux->value = ft_strdup(sign + 1);
		if (!aux->value)
			return (built_end("export", "malloc", NULL, '\0'));
		envp[0] = ft_strdup(new_env);
		if (!envp)
			return (built_end("export", "malloc", NULL, '\0'));
	}
	else
		return (0);
	ft_remove_arr(envp, 0);
	return (0);
}

static t_list *new_envp(char **new_env, t_list *head)
{
	t_list	*current;
	t_list	*next;
	t_var	*new_node;
	int			i;

	i = -1;
	while (new_env[++i])
	{
		new_node = create_envp(new_env[i]);
		if (!new_node)
			return (NULL);
		next = ft_lstnew(new_node);
		current = head;
		while (current->next)
			current = current->next;
		current->next = next;
	}
	return (head);
}


t_list	*b_export(char **envp, t_list *head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;
	int		j;

	tmp = head;
	j = 0;
	if (ft_isal_num(args, head))
		return (head);
	while (tmp)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (change_value_env(aux, envp, args[i]) == 1)
					return (NULL);
		tmp = tmp->next;
	}
	envp = shell_realloc(args, envp);
	if (!new_envp(args, head) || !envp)
		return (NULL);
	return (head);
}
