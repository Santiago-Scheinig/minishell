/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/21 14:38:01 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

char	**shell_pmtstr(t_list *envp)
{
	t_var	*tmp;
	char	**ps1;
	
	ps1 = malloc(2 * sizeof(char *));
	if (!ps1)
		return (NULL);//forcend
	while (envp)
	{
		tmp = (t_var *)envp->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			ps1[0] = ft_strdup("\\u:\\w\\$ ");
			if (!ps1[0])
				return (NULL);//forcend
			ps1[1] = NULL;
			tmp->value = ps1[0];
			return (ps1);
		}
		envp = envp->next;
	}
	ps1[0] = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	ps1[1] = NULL;
	return (ps1);
}
