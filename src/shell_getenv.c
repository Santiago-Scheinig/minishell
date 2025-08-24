/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_getenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:43:45 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 17:22:53 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_body	*init_env(t_body *minishell, char **envp)
{
	int		i;
	int		j;

	i = 0;
	while (envp[i])
		i++;
	minishell->envp = malloc((i + 1) * sizeof(char *));
	if (!minishell->envp)
		return (NULL);
	j = 0;
	while (j < i)
	{
		minishell->envp[j] = ft_strdup(envp[j]);
		if (!minishell->envp[j])
		{
			while (--j >= 0)
				free(minishell->envp[j]);
			free(minishell->envp);
			return (NULL);
		}
		j++;
	}
	minishell->envp[i] = NULL;
	return (minishell);
}

char	**ft_realloc(char **envp, char **new_envp, int size)
{
	char	**tmp;
	int		i;

	tmp = malloc((size + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	i = -1;
	while (envp[++i])
		ft_memmove(tmp, envp, (size) * sizeof(char *));
	i = -1;
	while (new_envp[++i])
	{
		tmp[size + i] = ft_strdup(new_envp[i]);
		if (!tmp[size])
			return (NULL);
	}
	tmp[size + i] = NULL;
	return (tmp);
	
}

char	*shell_getenv(t_body *minishell, const char *name)
{
	t_env	*tmp;

	tmp = minishell->lst_export;
	while (tmp)
	{
		if (!ft_strncmp(tmp->name, name, (ft_strlen(name) + 1)))
		{
			if (!tmp->value[0])
				return (NULL);
			return (tmp->value);
		}
		tmp = tmp->current_next;
	}
	return (NULL);
}
