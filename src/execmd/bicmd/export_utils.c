/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:38:52 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/06 19:08:41 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

int	set_equal(t_var *aux, char **envp, char *sign, char *new_env)
{
	size_t	len;
	int		i;
	
	i = 0;
	len = ft_strlen(aux->name);
	while ((ft_strlen(envp[i]) == len) && ft_strncmp(envp[i], aux->name, len))
		i++;
	if (aux->value)
		free(aux->value);
	free(envp[i]);
	aux->value = ft_strdup(sign + 1);
	if (!aux->value)
		return (built_end("export", "malloc", NULL, '\0'));
	envp[i] = ft_strdup(new_env);
	if (!envp)
		return (built_end("export", "malloc", NULL, '\0'));
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

char	**export_no_equal(char **args, char **envp)
{
	int	k;
	int	i;
	int	j;
	
	i = -1;
	while (args && args[++i])
	{
		j = i;
		while (envp[++j])
		{
			k = 0;
			while (args[i][k] && args[i][k] != '=')
				k++;
			if (!ft_strncmp(args[i], envp[j], k))
			{
				if (!ft_strchr(args[i], '='))
				{
					args = ft_remove_arr(args, i);
					i--;
				}
			}
		}
	}
	return (args);
}

char	**export_no_dup(char **args)
{
	size_t	k;
	size_t	p;
	int	i;
	int	j;
	
	i = -1;
	while (args && args[++i])
	{
		j = i;
		while (args[++j])
		{
			k = 0;
			while (args[i][k] && args[i][k] != '=')
				k++;
			p = 0;
			while (args[j][p] && args[j][p] != '=')
				p++;
			if (k == p && !ft_strncmp(args[i], args[j], k))
			{
				if (ft_strchr(args[j], '='))
					args = ft_remove_arr(args, i);
				else
					args = ft_remove_arr(args, j);
				j = i + 1;
			}
		}
	}
	return (args);
}

char	**shell_realloc(char **args, char **envp)
{
	size_t	old_size;
	size_t	new_size;
	size_t	args_len;
	size_t	i;
	char	**tmp;

	i = ft_arrlen((const void **)envp);
	args_len = ft_arrlen((const void **)args);
	new_size = (args_len + i + 1) * sizeof(char *);
	old_size = (ft_arrlen((const void **)envp) + 1) * sizeof(char *);
	tmp = ft_realloc(envp, new_size, old_size);	
	if (!tmp)
	{
		built_end("export", "malloc", NULL, '\0');
		return (NULL);
	}
	old_size = i;
	i = -1;
	while (++i < args_len)
		tmp[i + old_size] = ft_strdup(args[i]);
	tmp[i + old_size] = NULL;
	envp = tmp;
	return (envp);
}
