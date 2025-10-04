/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_import.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:12 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 15:02:09 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

static char	*change_list(char *args, t_list *head)
{
	t_list	*current;
	t_var	*tmp;
	char	*aux;

	current = head;
	while (current)
	{
		tmp = (t_var *)current->content;
		if (!ft_strncmp(tmp->name, args, ft_strlen(tmp->name)))
		{
			aux = ft_strchr(args, '=');
			if (aux)
			{
				free(tmp->value);
				tmp->value = ft_strdup(aux + 1);
				if (!tmp->name)
				{
					built_end("export", "System failed", NULL, '\0');
					return (NULL);
				}
			}
		}
		current = current->next;
	}
	return (args);
}

static char	**change_envp(char ***envp, char **args, int i, int j)
{
	if (ft_strchr(args[i], '='))
	{
		free(envp[0][j]);
		envp[0][j] = ft_strdup(args[i]);
		if (!envp[0][j])
		{
			built_end("export", "System failed", NULL, '\0');
			return (NULL);
		}
		args = ft_remove_arr(args, i);
	}
	return (args);
}

static char	**check_argv(char ***envp, char **args, t_list *head)
{
	size_t	k;
	size_t	p;
	int		i;
	int		j;

	i = -1;
	while (args && args[++i] && envp[0])
	{
		j = -1;
		while (envp[0][++j])
		{
			if (args[i] != NULL && args[i][0] != '\0')
				k = ft_strlenchr(args[i], '=');
			p = ft_strlenchr(envp[0][j], '=');
			if (k == p && !ft_strncmp(args[i], envp[0][j], k))
			{
				change_list(args[i], head);
				change_envp(envp, args, i, j);
			}
		}
	}
	return (args);
}

static char	**check_args(char **args)
{
	size_t	invalid;
	size_t	i;
	size_t	j;

	i = 0;
	invalid = 0;
	while (args && args[i] && ft_strchr(args[i], '='))
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
			invalid = 1;
		j = 1;
		while ((ft_isalnum(args[i][j]) || args[i][j] == '_')
			&& args[i][j] != '=')
			j++;
		if (args[i][j] != '=')
			invalid = 1;
		i++;
	}
	if (invalid == 1)
		return (NULL);
	if (args)
		args = export_no_dup(args);
	return (args);
}

/**
 * Imports variables into envp/list from argument list.
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param head Pointer-to-list head of current variables.
 * @param args Array of argument strings to import ("NAME" or "NAME=VALUE").
 * 
 * Validates identifiers, updates existing entries, and appends new ones.
 * 
 * @return 0 on success, non-zero on failure.
 */
int	msh_import(char ***envp, t_list **head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;

	tmp = *head;
	args = check_args(args);
	if (!args)
		return (1);
	args = check_argv(envp, args, *head);
	if (!args)
		return (1);
	while (tmp && tmp->content)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (!change_value_env(&envp[0], &args[i], aux, 0))
					args = ft_remove_arr(&args[0], i);
		tmp = tmp->next;
	}
	if (new_envp(args, head, 0))
		return (1);
	return (0);
}
