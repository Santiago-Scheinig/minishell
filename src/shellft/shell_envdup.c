/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envdup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 19:13:22 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/31 16:33:06 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
