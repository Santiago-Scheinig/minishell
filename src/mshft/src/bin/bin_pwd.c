/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:04:59 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/03 17:32:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Implements the 'pwd' built-in command.
 *
 *			Prints the current working directory to standard output.
 *			If getcwd() fails, retrieves the last known directory from
 *			the environment variable 'PWD' instead.
 *
 * @param	args	Array of command arguments. Supports no options
 *					except rejecting flags starting with '-'.
 * @param	lst_t_var	Linked list of environment variables used
 *					to resolve 'PWD' when getcwd() fails.
 *
 * @note	Uses getcwd() to retrieve the absolute path. If it fails,
 *			the fallback environment value is printed instead.
 * @note	Returns an error through shell_builterr() if a write
 *			or memory allocation fails.
 *
 * @return	0 on success, or an error code if a system call or write
 *			fails.
 */
int	bin_pwd(char **args, t_list *lst_t_var)
{
	char	*path;

	path = NULL;
	if (args[1] && args[1][0] == '-')
		return (shell_binerr(INVFLGS, "pwd", "[]", args[1][1]));
	path = getcwd(NULL, 0);
	if (!path)
	{
		path = ft_strdup(shell_envchr(NULL, "PWD", lst_t_var));
		if (!path)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, path, ft_strlen(path)) == -1)
			return (shell_binerr(SYSFAIL, "pwd", "write", '\0'));
	}
	else
	{
		if (write(STDOUT_FILENO, path, ft_strlen(path)) == -1)
			return (shell_binerr(SYSFAIL, "pwd", "write", '\0'));
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (shell_binerr(SYSFAIL, "pwd", "write", '\0'));
	}
	free(path);
	return (0);
}
