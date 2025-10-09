/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:59:36 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 06:00:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

static int	print_t_var(char *value)
{
	if (value)
	{
		if (write(STDOUT_FILENO, "=\"", 2) == -1)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, value, ft_strlen(value)) == -1)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, "\"", 1) == -1)
			return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * Builterr
 */
int	print_lst_t_var(t_list *lst_t_var)
{
	t_var	*var;
	
	while (lst_t_var && lst_t_var->content)
	{
		var = (t_var *)lst_t_var->content;
		if (var->name && var->exported)
		{
			if (write(STDOUT_FILENO, "declare -x ", 11) == -1)
				return (built_end("export", "System failed", NULL, '\0'));//modif
			if (write(STDOUT_FILENO, var->name, ft_strlen(var->name)) == -1)
				return (built_end("export", "System failed", NULL, '\0'));//modif
			if (print_t_var(var->value))
				return (built_end("export", "System failed", NULL, '\0'));//modif
			if (write(STDOUT_FILENO, "\n", 1) == -1)
				return (built_end("export", "System failed", NULL, '\0'));//modif
		}
		lst_t_var = lst_t_var->next;
	}
	return (MSHELL_SUCCESS);
}

/**
 * Builterr
 */
int	is_valid_var(char *arg, int print)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
	{
		if (print)
			return (built_end("export", "Not valid identifier", arg, '\0'));
		else
			return (1);
	}
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
		{
			if (print)
				return (built_end("export", "Not valid identifier", arg, '\0'));
			else
				return (1);
		}
		i++;
	}
	return (0);
}

char	**ft_remove_arr(char **arr, int index)
{
	int		len;

	if (!arr)
		return (arr);
	len = ft_arrlen((const void **)arr);
	if (index >= len)
		return (arr);
	free(arr[index]);
	while (index < len - 1)
	{
		if (arr[index + 1] != NULL)
			arr[index] = arr[index + 1];
		else
			arr[index] = NULL;
		index++;
	}
	arr[len - 1] = NULL;
	return (arr);
}
