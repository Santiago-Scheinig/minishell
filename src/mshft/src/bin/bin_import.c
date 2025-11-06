/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_import.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:12 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/18 20:33:14 by sscheini         ###   ########.fr       */
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
 * @brief	Validates and sanitizes arguments for the 'import' builtin.
 *
 *			Checks that each argument in argv is a valid variable
 *			assignment containing an '=' symbol. If any invalid entry
 *			is found, all arguments up to that point are removed from
 *			the array.
 *
 * @param	argv	Double pointer to the argument array.
 *
 * @note	Removes invalid entries using ft_remove_arr() to prevent
 *			inconsistent variable states.
 * @note	A return value of 1 indicates invalid or malformed input.
 *
 * @return	1 if invalid arguments were found, or 0 if all are valid.
 */
static int	check_arguments(char ***argv)
{
	int	i;
	int	j;

	if (!(*argv))
		return (MSHELL_FAILURE);
	i = -1;
	while ((*argv)[++i])
	{
		if (!ft_strchr((*argv)[i], '=') || is_valid_var((*argv)[i], 0))
		{
			j = -1;
			while (++j <= i)
				ft_remove_arr((*argv), j);
			return (MSHELL_FAILURE);
		}
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Implements the 'import' built-in command.
 *
 *			Adds or updates variables from argv into the shell's
 *			environment list and array. Before importing, validates
 *			all entries using check_arguments() to ensure correct
 *			format (VAR=VALUE).
 *
 *			Static helpers:
 *
 *				- get_name():			Extracts variable name from
 *										an assignment string.
 *				- check_arguments():	Validates syntax of import
 *										arguments and removes invalid
 *										entries.
 *
 * @param	argv	Double pointer to the array of assignment strings
 *					to import into the environment.
 * @param	envp	Double pointer to the current environment array
 *					used by execve() and child processes.
 * @param	lst_envp	Linked list representing the shell's
 *					environment variables.
 *
 * @note	Existing variables are updated using shell_envlst_swp(),
 *			while new ones are inserted via shell_envlst_add().
 * @note	Returns MSHELL_FAILURE on syntax errors or allocation
 *			failures.
 *
 * @return	MSHELL_SUCCESS on success, or error code on failure.
 */
int	bin_import(char ***argv, char ***envp, t_list *lst_envp)
{
	char	*envar_name;
	int		swp;
	int		exp;
	int		i;

	if (check_arguments(argv))
		return (MSHELL_FAILURE);
	i = -1;
	swp = false;
	while ((*argv)[++i])
	{
		envar_name = get_name((*argv)[i]);
		if (!envar_name)
			return (shell_binerr(SYSFAIL, "import", "malloc", 0));
		if (shell_envchr(&exp, envar_name, lst_envp))
			swp = true;
		free(envar_name);
		if (swp && shell_envlst_swp(exp, (*argv)[i], envp, lst_envp))
			return (shell_binerr(SYSFAIL, "import", "malloc", 0));
		if (!swp && shell_envlst_add(exp, (*argv)[i], envp, lst_envp))
			return (shell_binerr(SYSFAIL, "import", "malloc", 0));
	}
	return (MSHELL_SUCCESS);
}
