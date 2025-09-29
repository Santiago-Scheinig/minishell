/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/29 14:42:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static int	change_value(char *envp)
{
	char	*value;
	char	*tmp;

	tmp = ft_strdup(&envp[6]);
	if (!tmp)
		return (-1);
	value = ft_atoi(tmp) + 1;
	free(tmp);
	return (value);
}

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
			value = change_value(envp);
			if (value == -1)
				return (NULL);
		}
		else
			value = 1;
		tmp = ft_strdup("SHLVL=");
		numb = ft_itoa(value);
		aux = ft_strjoin(tmp, numb);
		free(tmp);
		free(numb);
		if (!aux)
			return (NULL);
		return (aux);
	}
	return (NULL);
}

/**
 * Duplicates the process environment array and adjusts SHLVL if present.
 * 
 * @param envp Null-terminated array of environment strings from the parent
 * 		process.
 * 
 * Creates and returns a newly allocated copy of envp. If a "SHLVL=" entry is
 * found the function attempts to increment its numeric value; if the existing
 * value is invalid it sets SHLVL to 1 in the duplicated array.
 * 
 * @return Newly allocated null-terminated array of strings, or NULL on
 * 		allocation failure.
 * @note - Caller is responsible for freeing the returned array
 * 		(each string and the array).
 * @note - On allocation failure any partial allocations are freed before
 * 		returning NULL.
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
				return (NULL);
			}
		}
	}
	tmp[i] = NULL;
	return (tmp);
}
