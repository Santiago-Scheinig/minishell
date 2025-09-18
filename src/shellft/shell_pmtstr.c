/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 17:30:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

char	*shell_pmtstr(t_list *envp)
{
	t_var	*tmp;
	char	**ps1;
	
	while (envp)
	{
		tmp = (t_var *)envp->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			tmp->value = ft_strdup("\\u:\\w\\$ ");
			if (!tmp->value)
				return (NULL);//forcend
			return (tmp->value);
		}
		envp = envp->next;
	}
	ps1 = malloc(2 * sizeof(char *));
	if (!ps1)
		return (NULL);//forcend
	ps1[0] = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	ps1[1] = NULL;
	inport(&minishell->envp, (t_list *)minishell->envp_lst, ps1);
	free(ps1[0]);
	free(ps1);
	return (minishell->envp[0]);
}
