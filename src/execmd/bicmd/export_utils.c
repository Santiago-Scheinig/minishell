/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:38:52 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/31 20:13:12 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

char	**ft_remove_arr(char **arr, size_t index)
{
	size_t	i;
	size_t	len;
	char	**tmp;

	if (!arr)
		return (arr);
	len = ft_arrlen((const void **)arr);
	if (index >= len)
		return (arr);
	free(arr[index]);
	i = index;
	while (i < len - 1)
		arr[i] = arr[i + 1];
	arr[len - 1] = NULL;
	tmp = ft_realloc(arr, (len - 1) * sizeof(char *), len * sizeof(char *));
	if (tmp)
		arr = tmp;
	return (arr);
}

char	**shell_realloc(char **args, char **envp)
{
	size_t	old_size;
	size_t	new_size;
	size_t	args_len;
	size_t	i;
	char	**tmp;

	args_len = ft_arrlen((const void **)args);
	old_size = (ft_arrlen((const void **)envp) + 1) * sizeof(char *);
	new_size = args_len + (ft_arrlen((const void **)envp) + 1) * sizeof(char *);
	i = ft_arrlen((const void **)envp);
	tmp = ft_realloc(envp, new_size, old_size);	
	if (!tmp)
	{
		built_end("export", "malloc", NULL, '\0');
		return (NULL);
	}
	envp = tmp;
	old_size = i;
	i = 0;
	while (i++ < args_len)
		envp[i + old_size - 1] = ft_strdup(args[i - 1]);
	new_size = ft_arrlen((const void **)envp);
	envp[new_size] = NULL;
	return (envp);
}
