/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/03 21:55:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * COMMENT PENDING
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
	j = 0;
	while (j < i)
	{
		tmp[j] = ft_strdup(envp[j]);
		if (!tmp[j])
		{
			while (--j >= 0)
				free(tmp[j]);
			free(tmp);
			return (NULL);
		}
		j++;
	}
	tmp[i] = NULL;
	return (tmp);
}
