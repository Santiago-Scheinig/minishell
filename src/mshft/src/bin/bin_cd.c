/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:07 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/05 15:27:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Updates the "PWD" variable in the environment list.
 *
 *			Obtains the current directory path using getcwd(), duplicates
 *			it, and replaces the existing value of "PWD" in the env list.
 *			Called after a successful directory change in bin_cd().
 *
 * @param	envp	Double pointer to the environment variable list.
 *
 * @note	Allocates memory for the new path string. Frees the old
 *			value stored in the "PWD" variable before replacing it.
 * @note	If memory allocation fails, calls shell_binerr() to handle
 *			the system error.
 *
 * @return	0 on success, or system error code on failure.
 */
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
		return (shell_binerr(SYSFAIL, "cd", "malloc", 0));
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
 * @brief	Executes the 'cd' built-in command to change directory.
 *
 *			Updates the current working directory of the shell process.
 *			If no path argument is given, changes to the user's home dir.
 *			On success, updates the 'PWD' variable in the environment list.
 *
 *			Static helper:
 *
 *				- change_pwd():	Finds and updates the 'PWD' variable
 *								in the environment list after a
 *								successful directory change.
 *
 * @param	args	Argument vector from user input. The first element
 *					is "cd", followed by optional directory argument.
 * @param	envp	Double pointer to the environment variable list.
 *
 * @note	If no HOME variable exists, an error is raised.
 * @note	Supports only one argument; additional args cause error.
 * @note	The current directory is changed using chdir(), and on
 *			success, the new path is stored in the "PWD" variable.
 *
 * @return	0 on success, or error code returned by shell_binerr() on
 *			failure.
 */
int	bin_cd(char **args, t_list **envp)
{
	char	*dir;

	if (!args[1])
	{
		dir = shell_envchr(NULL, "HOME", *envp);
		if (!dir)
			return (shell_binerr(INVHOME, "cd", "malloc", '\0'));
	}
	else if (args[1] && args[1][0] == '-')
		return (shell_binerr(INVFLGS, "cd", "dir", args[1][1]));
	else if (!args[2])
		dir = args[1];
	else
		return (shell_binerr(INVARGC, "cd", "malloc", '\0'));
	if (chdir(dir) != 0)
		return (shell_binerr(SYSFAIL, "cd", dir, '\0'));
	return (change_pwd(envp));
}
