/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 20:58:33 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

/**
 * @brief Initializes the PWD environment variable with the current working
 *        directory.
 *
 * Uses getcwd() to retrieve the current directory and creates a new string
 * "PWD=<current_directory>".
 *
 * @note    Memory is allocated for the new string. Caller is responsible for
 *          freeing it.
 * @note    Frees the temporary getcwd() result after use.
 *
 * @return  Pointer to the newly allocated PWD string, or NULL on allocation
 *          failure.
 */
static char	*pwd_init(void)
{
	char	*pwd;
	char	*aux;

	aux = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", aux);
	free(aux);
	return (pwd);
}

/**
 * @brief Initializes the last executed command environment variable (_).
 *
 * Retrieves the value of the "_" environment variable from lst_t_var.
 * If it exists, creates a new string "_=<value>". Otherwise, sets it to
 * the default "_=/minishell".
 *
 * @param lst_t_var	Pointer to the environment variable linked list
 * 					(t_list of t_var).
 *
 * @note	Memory is allocated for the new string. Caller is responsible
 *			for freeing it.
 *
 * @return	Pointer to the newly allocated last command string, or NULL
 *			on allocation failure.
 */
static char	*lastcmd_init(t_list *lst_t_var)
{
	char	*last_cmd;
	char	*aux;

	aux = shell_getenv(lst_t_var, "_");
	if (!aux)
		last_cmd = ft_strjoin("_=", "/minishell");
	else
		last_cmd = ft_strjoin("_=", aux);
	return (last_cmd);
}

/**
 * @brief Initializes or resets the PS1 environment variable.
 *
 * Searches the linked list of environment variables (lst_t_var) for "PS1".
 * If found, frees its current value and sets it to the default prompt string
 * "\\u:\\w\\$ ". If not found, returns a new string "PS1=\\u:\\w\\$ ".
 *
 * @param lst_t_var	Pointer to the environment variable linked list
 * 					(t_list of t_var).
 *
 * @note	Memory is allocated for the new prompt string. Caller is responsible
 *			for freeing it if needed.
 * @note	Updates the existing t_var->value if PS1 exists.
 *
 * @return	Pointer to the new PS1 string, or NULL on allocation failure.
 */
static char	*ps1_init(t_list *lst_t_var)
{
	t_var	*tmp;
	char	*ps1;

	while (lst_t_var && lst_t_var->content)
	{
		tmp = (t_var *)lst_t_var->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			ps1 = ft_strdup("\\u:\\w\\$ ");
			if (!ps1)
				return (NULL);
			tmp->value = ps1;
			return (ps1);
		}
		lst_t_var = lst_t_var->next;
	}
	ps1 = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	return (ps1);
}

/**
 * @brief Creates an array of strings representing key shell prompt variables.
 *
 * Allocates an array of 5 strings and populates it with:
 *   0: PS1 prompt string (from ps1_init)
 *   1: Current working directory (from pwd_init)
 *   2: Last executed command (from lastcmd_init)
 *   3: SHLVL=1 string
 *   4: NULL terminator
 *
 * @param lst_t_var	Pointer to the environment variable linked list
 * 					(t_list of t_var).
 *
 * @note	Memory is allocated for both the array and each string. Caller
 *			is responsible for freeing them.
 * @note	Returns NULL if any allocation fails.
 *
 * @return Pointer to the dynamically allocated array of prompt-related strings,
 *			or NULL on failure.
 */
char	**shell_pmtstr(t_list *lst_t_var)
{
	char	**ps;

	ps = malloc(5 * sizeof(char *));
	if (!ps)
		return (NULL);
	ps[0] = ps1_init(lst_t_var);
	if (!ps[0])
		return (NULL);
	ps[1] = pwd_init();
	if (!ps[1])
		return (NULL);
	ps[2] = lastcmd_init(lst_t_var);
	if (!ps[2])
		return (NULL);
	ps[3] = ft_strjoin("SHLVL=", "1");
	if (!ps[3])
		return (NULL);
	ps[4] = NULL;
	return (ps);
}
