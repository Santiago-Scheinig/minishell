/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/12/15 15:11:16 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"

/**
 * @brief	Extracts the variable name from a VAR=VALUE string.
 *
 *			Searches the provided environment assignment for the first
 *			'=' character and returns a substring containing only the
 *			variable name portion.
 *
 * @param	var		String representing an environment assignment (e.g.
 *					"PATH=/usr/bin").
 *
 * @note	The returned string is dynamically allocated and must be freed
 *			by the caller.
 * @note	If no '=' character is found, returns a copy of the full
 *			string up to the null terminator.
 *
 * @return	Pointer to the allocated variable name, or NULL on malloc
 *			failure.
 */
static char	*get_name(char *var)
{
	char	*name;
	int		i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	name = ft_substr(var, 0, i);
	if (!name)
		return (NULL);
	name[i] = '\0';
	return (name);
}

/**
 * @brief	Implements the 'export' built-in command.
 *
 *			Adds or updates environment variables in the shell's
 *			environment list and array. If no arguments are given,
 *			prints the current environment variables instead.
 *
 *			Static helper:
 *				- get_name():	Extracts variable names from "VAR=VAL"
 *								assignments for update or insertion.
 *
 * @param	argv		Array of strings containing variable assignments or
 *						arguments for export.
 * @param	envp		Double pointer to the current environment array used
 *						by execve() and other commands.
 * @param	lst_envp	Linked list representing environment variables in
 *						the shell context.
 *
 * @note	If a variable already exists, it is updated in-place using
 *			shell_envlst_swp(). Otherwise, shell_envlst_add() inserts it.
 * @note	If called without arguments, it invokes print_lst_t_var() to
 *			display all variables.
 * @note	On allocation or system call failures, an appropriate error is
 *			reported via shell_builterr().
 *
 * @return	MSHELL_SUCCESS on success, or an error code on failure.
 */
int	bin_export(char **argv, char ***envp, t_list *lst_envp)
{
	char	*envar_name;
	int		swp;
	int		i;

	if (!argv[0])
		return (print_lst_t_var(lst_envp));
	i = -1;
	swp = false;
	while (argv[++i])
	{
		if (is_valid_var(argv[i], true))
			continue ;
		envar_name = get_name(argv[i]);
		if (!envar_name)
			return (shell_binerr(SYSFAIL, "export", "malloc", 0));
		if (shell_envchr(NULL, envar_name, lst_envp))
			swp = true;
		free(envar_name);
		if (swp && shell_envlst_swp(true, argv[i], envp, lst_envp))
			return (shell_binerr(SYSFAIL, "export", "malloc", 0));
		if (!swp && shell_envlst_add(true, argv[i], envp, lst_envp))
			return (shell_binerr(SYSFAIL, "export", "malloc", 0));
	}
	return (MSHELL_SUCCESS);
}
