/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/29 19:44:21 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

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

static int	ft_isal_num(char **new_e, t_list *head)
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

static t_var	*change_value_env(t_var *aux, char *new_env)
{
	char	*sign;

	sign = ft_strchr(new_env, '=');
	if (!sign)
		return (NULL);
	free(aux->value);
	aux->value = ft_strdup(sign + 1);
	if (!aux->value)
		built_end("export", "malloc", NULL, '\0');
	return (aux);
}

static t_var *new_envp(char **new_env, t_list *head)
{
	t_list	*current;
	t_var	*new_node;
	int			error;
	int			i;

	i = -1;
	error = 0;
	while (new_env[++i])
	{
		new_node = create_envp(new_env[i], &error);
		if (!new_node)
			return (NULL);
		current = head;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (head);
}

t_var	*export(char **envp, t_list *head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;
	int		j;

	tmp = head;
	j = 0;
	if (ft_isal_num(args, head) == 2)
		return (head);
	while (tmp)
	{
		i = 1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name) + 1))
				aux = change_value_env(aux, args[i]);
		tmp = tmp->next;
		j++;
	}
	envp = ft_realloc(envp, &args[1], j + i - 1);
	new_envp(&args[1], head);
	return (head);
}
