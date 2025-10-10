/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 09:18:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

static int	change_pwd(t_list **envp)
{
	t_list	*current;
	t_var	*tmp;
	char	*pwd;
	char	*aux;

	aux = getcwd(NULL, 0);
	if (!aux)
		return (0);
	pwd = ft_strdup(aux);
	free(aux);
	if (!pwd)
		return (shell_builterr(SYSFAIL, "cd", NULL, 0));
	current = *envp;
	while (current && current->content)
	{
		tmp = (t_var *)current->content;
		if (!ft_strncmp(tmp->name, "PWD", 3))
		{
			free(tmp->value);
			tmp->value = pwd;
		}
		current = current->next;
	}
	return (0);
}

/**
 * Builterr
 */
int	bin_cd(char **args, t_list **envp)
{
	char	*dir;

	if (!args[1])
	{
		dir = shell_envchr(NULL, *envp, "HOME");
		if (!dir)
			return (shell_builterr(INVHOME, "cd", NULL, 0));
	}
	else if (args[1] && args[1][0] == '-')
		return (shell_builterr(INVFLGS, "cd", "[dir]", args[1][1]));//que es lo que va entre corchetes?
	else if (!args[2])
		dir = args[1];
	else
		return (shell_builterr(INVARGC, "cd", NULL, '\0'));
	if (chdir(dir) != 0)
		return (shell_builterr(SYSFAIL, "cd", NULL, '\0'));//Esto es fatal? Hay que terminar la msh si es asi.
	return (change_pwd(envp));
}
