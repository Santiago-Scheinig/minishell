/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/06 19:06:48 by ischeini         ###   ########.fr       */
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

static char	**ft_isal_num(char **args, char **envp, t_list *head)
{
	int	i;
	int	j;

	i = 0;
	if (!args[0])
	{
		print_export(head);
		return (NULL);	
	}
	if (args[0][0] == '-' && args[0][1])
	{
		built_end("export", "Invalid flags", "name[=value ...]", args[0][1]);
		return (NULL);
	}
	j = -1;
	while (args[++j])
	{
		if (!is_valid_identifier(args[j]) && args[j][i] != '_')
			args = ft_remove_arr(&args[0], j);
	}
	args = export_no_dup(args);
	args = export_no_equal(args, envp);
	return (args);
}

static int	change_value_env(t_var *aux, char **envp, char *new_env)
{
	size_t	i;
	size_t	j;
	char	*sign;

	i = ft_strlen(aux->name);
	j = 0;
	while (new_env[j] && new_env[j] != '=')
		j++;
	if (j != i)
		return (1);
	sign = ft_strchr(new_env, '=');
	if (sign)
		set_equal(aux, &envp[0], sign, new_env);
	return (0);
}

static t_list *new_envp(char **new_env, t_list *head)
{
	t_list	*current;
	t_list	*next;
	t_var	*new_node;
	int			i;

	i = 0;
	while (new_env[i])
	{
		new_node = create_envp(new_env[i]);
		if (!new_node)
			return (NULL);
		next = ft_lstnew(new_node);
		current = head;
		while (current->next)
			current = current->next;
		current->next = next;
		i++;
	}
	return (head);
}


t_list	*b_export(char ***envp, t_list *head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;
	int		j;

	tmp = head;
	j = 0;
	args = ft_isal_num(args, envp[0], head);
	if (!args)
		return (head);
	while (tmp)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (!change_value_env(aux, &envp[0][0], args[i]))
					args = ft_remove_arr(&args[0], i);
		tmp = tmp->next;
	}
	envp[0] = shell_realloc(args, envp[0]);
	if (!new_envp(args, head) || !envp[0])
		return (NULL);
	return (head);
}
