/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 18:17:58 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

char	**shell_pmtstr(t_list *envp_lst)
{
	t_var	*tmp;
	char	**ps1;
	
	while (envp_lst)
	{
		tmp = (t_var *)envp_lst->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			tmp->value = ft_strdup("\\u:\\w\\$ ");
			if (!tmp->value)
				return (NULL);//forcend
			return (&tmp->value);
		}
		envp_lst = envp_lst->next;
	}
	ps1 = malloc(2 * sizeof(char *));
	if (!ps1)
		return (NULL);//forcend
	ps1[0] = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	ps1[1] = NULL;
	return (ps1);
}
