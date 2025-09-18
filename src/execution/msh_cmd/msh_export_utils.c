/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:38:52 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:40:08 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

int	set_equal(t_var *aux, char **envp, char *sign, char *new_env)
{
	size_t	len;
	int		i;

	i = 0;
	len = ft_strlen(aux->name);
	if (!envp[i])
		return (0);
	while ((ft_strlen(envp[i]) == len) && ft_strncmp(envp[i], aux->name, len))
		i++;
	if (aux->value)
		free(aux->value);
	free(envp[i]);
	aux->value = ft_strdup(sign + 1);
	if (!aux->value)
		return (built_end("export", "System failed", NULL, '\0'));
	envp[i] = ft_strdup(new_env);
	if (!envp)
		return (built_end("export", "System failed", NULL, '\0'));
	return (0);
}

char	**export_no_equal(char **args, t_list *lst)
{
	t_list	*current;
	t_var	*var;
	int		k;
	int		i;

	i = -1;
	while (args && args[++i])
	{
		current = lst;
		while (current)
		{
			var = (t_var *)current->content;
			k = 0;
			while (args[i][k] && args[i][k] != '=')
				k++;
			if (!ft_strncmp(args[i], var->name, ft_strlen(var->name)))
				if (!ft_strchr(args[i], '='))
				{
					var->exported = 1;
					args = ft_remove_arr(args, i--);
				}
			current = current->next;
		}
	}
	return (args);
}

char	**export_no_dup(char **args)
{
	size_t	k;
	size_t	p;
	int		i;
	int		j;

	i = -1;
	while (args && args[++i])
	{
		j = i;
		while (args[++j])
		{
			k = ft_strlenchr(args[i], '=');
			p = ft_strlenchr(args[j], '=');
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

char	**exp_resize(char **args, char **envp, size_t size)
{
	size_t	old_size;
	size_t	new_size;
	size_t	args_len;
	size_t	envp_len;
	char	**tmp;

	args_len = size;
	envp_len = ft_arrlen((const void **)envp);
	new_size = (args_len + envp_len + 1) * sizeof(char *);
	old_size = (ft_arrlen((const void **)envp) + 1) * sizeof(char *);
	tmp = ft_realloc(envp, new_size, old_size);
	if (!tmp)
	{
		built_end("export", "System failed", NULL, '\0');
		return (NULL);
	}
	old_size = envp_len;
	envp_len = -1;
	while (++envp_len < args_len)
		tmp[envp_len + old_size] = ft_strdup(args[envp_len]);
	tmp[envp_len + old_size] = NULL;
	return (tmp);
}
