/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 20:02:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static size_t	is_valid(const char *str)
{
	size_t	i;

	i = 6;
	while (str[i] && ft_isdigit(str[i]))
		i++;
	if (i == ft_strlen(str) && str[i - 1] != '=')
		return (1);
	return (0);
}

static char	*check_value(const char *envp)
{
	size_t	value;
	char	*numb;
	char	*aux;
	char	*tmp;

	if (!ft_strncmp(envp, "SHLVL=", 6))
	{
		if (is_valid(envp))
		{
			tmp = ft_strdup(&envp[6]);
			if (!tmp)
				return (NULL); //forcend
			value = ft_atoi(tmp) + 1;
			free(tmp);
		}
		else
			value = 1;
		tmp = ft_strdup("SHLVL=");
		numb = ft_itoa(value);
		aux = ft_strjoin(tmp, numb);
		free(tmp);
		free(numb);
		if (!aux)
			return (NULL); //forcend
		return (aux);
	}
	return (NULL);
}

/**
 * COMMENT PENDING ISMA
 */
char	**shell_envpdup(const char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	i = ft_arrlen((const void **)envp);
	tmp = malloc((i + 1) * sizeof(char *));
	if (!tmp)
		return (NULL);
	j = -1;
	while (++j < i)
	{
		tmp[j] = check_value(envp[j]);
		if (!tmp[j])
		{
			tmp[j] = ft_strdup(envp[j]);
			if (!tmp[j])
			{
				while (--j >= 0)
					free(tmp[j]);
				free(tmp);
				return (NULL); //forcend
			}
		}
	}
	tmp[i] = NULL;
	return (tmp);
}
