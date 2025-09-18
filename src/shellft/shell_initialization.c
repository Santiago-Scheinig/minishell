/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_initialization.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 14:04:14 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "troubleshoot.h"
#include "minishell.h"
#include "parser.h"
#include "bicmd.h"

static char	*init_ps1(t_body *minishell)
{
	t_list	*new_node;
	t_var	*tmp;
	char	**ps1;
	
	new_node = minishell->envp_lst;
	while (new_node)
	{
		tmp = (t_var *)new_node->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			tmp->value = ft_strdup("\\u:\\w\\$ ");
			return (tmp->value);
		}
		new_node = new_node->next;
	}
	ps1 = malloc(2 * sizeof(char *));
	if (!ps1)
		return (NULL);
	ps1[0] = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	ps1[1] = NULL;
	inport(&minishell->envp, (t_list *)minishell->envp_lst, ps1);
	free(ps1[0]);
	free(ps1);
	return (minishell->envp[0]);
}

void	init_shell(t_body *minishell, const char **envp)
{
	char	*tmp;

	minishell->envp = shell_envpdup(envp);
	if (!minishell->envp)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	minishell->envp_lst = shell_newlst_var(minishell->envp);
	sortenv(&minishell->envp_lst);
	if (!minishell->envp_lst)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	tmp = shell_prompt(minishell);
	if (!tmp)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	free(tmp);
	if (!init_ps1(minishell))
		forcend(minishell, "malloc", MSHELL_FAILURE);
	sortenv(&minishell->envp_lst);
}

