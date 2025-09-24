/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 20:14:39 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

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
		return( built_end("cd", "System failed", NULL, '\0'));
	current = *envp;
	while (current)
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
 * Built-in 'cd' command for msh.
 * 
 * @param args Argument array for cd (args[0] is "cd").
 * @param envp Pointer-to-list head of environment variables
 * (used to update PWD).
 * 
 * Changes the working directory to the specified path or HOME if no arg
 * provided.
 * Reports errors and returns corresponding error.
 * 
 * @return 0 on success, non-zero on error.
 * @note - Print errors directly.
 */
int	msh_cd(char **args, t_list **envp)
{
	char	*dir;

	if (!args[1])
	{
		dir = shell_getenv(*envp, "HOME");
		if (!dir)
			return (built_end(args[0], "HOME", NULL, '\0'));
	}
	else if (args[1] && args[1][0] == '-')
	{
		return (built_end(args[0], "Invalid flags", "[dir]", args[1][1]));
	}
	else if (!args[2])
		dir = args[1];
	else
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	if (chdir(dir) != 0)
		return (built_end(args[0], "System failed", NULL, '\0'));
	return (change_pwd(envp));
}
