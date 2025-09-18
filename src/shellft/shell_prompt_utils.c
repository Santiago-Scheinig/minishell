/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_prompt_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 15:32:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/17 16:38:23 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "bicmd.h"

char	*transform_format(char *tmp, char *ps1, char *user, char *path)
{
	size_t	j;
	size_t	i;

	i = 0;
	j = 0;
	while (ps1 && ps1[i])
	{
		if (ps1[i] == '\\' && ps1[i + 1] && ps1[i + 1] != '\\')
		{
			if (ps1[i + 1] == 'u')
			{
				ft_memcpy(&tmp[j], user, ft_strlen(user));
				j += ft_strlen(user);
			}
			else if (ps1[i + 1] == 'w')
			{
				ft_memcpy(&tmp[j], path, ft_strlen(path));
				j += ft_strlen(path);
			}
			else
				tmp[j++] = ps1[i + 1];
			i += 2;
		}
		else
			tmp[j++] = ps1[i++];
	}
	tmp[j] = '\0';
	return (tmp);
}

size_t	prompt_len(char *ps1, char *user, char *path)
{
	size_t	len;
	size_t	i;

	i = 0;
	len = 0;
	while (ps1 && ps1[i])
	{
		if (ps1[i] == '\\' && ps1[i + 1] && ps1[i + 1] != '\\')
		{
			if (ps1[i + 1] == 'u')
				len += ft_strlen(user);
			else if (ps1[i + 1] == 'w')
				len += ft_strlen(path);
			else
				len += 1;
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
