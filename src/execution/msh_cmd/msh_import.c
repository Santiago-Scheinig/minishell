/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_import.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:12 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/21 14:33:25 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

static int	check_args(char **args)
{
	size_t	invalid;
	size_t	i;
	size_t	j;

	i = 0;
	invalid = 0;
	while (args[i] && ft_strchr(args[i], '='))
	{
		if (!ft_isalpha(args[i][0]) && args[i][0] != '_')
			invalid = 1;
		j = 1;
		while ((ft_isalnum(args[i][j]) || args[i][j] == '_') && args[i][j] != '=')
			j++;
		if (args[i][j] != '=')
			invalid = 1;
		i++;
	}
	if (invalid == 1)
		return (0);
	return (1);
}

int	msh_import(char ***envp, t_list **head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;

	tmp = *head;
	if (!check_args(args))
		return (0);
	if (!args)
		return (0);
	while (tmp)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (!change_value_env(aux, &envp[0], args[i], 0))
					args = ft_remove_arr(&args[0], i);
		tmp = tmp->next;
	}
	if (!new_envp(args, *head, 0))
		return (1);
	return (0);
}
