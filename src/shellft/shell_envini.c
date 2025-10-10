/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_envini.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 23:44:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"
#include "shell_std_utils.h"

static char	*cmdlog_init(char ***envp, t_list *lst_envp)
{
	int		exp;
	char	*cmdlog;

	exp = false;
	cmdlog = shell_envchr(&exp, "_", lst_envp);
	if (!cmdlog)
		if (shell_envlst_add(exp, "_=minishell", envp, lst_envp))
			return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

static char	*shlvl_init(char ***envp, t_list *lst_envp)
{
	char	*shlvl;
	int		exp;

	exp = false;
	shlvl = "SHLVL=1";
	if (shell_envchr(&exp, lst_envp, "SHLVL"))
		return (MSHELL_SUCCESS);
	else
	{
		if (shell_envlst_add(false, shlvl, envp, lst_envp))
			return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

static char	*pwd_init(char ***envp, t_list *lst_envp)
{
	char	*pwd;
	int		exp;

	exp = false;
	pwd = pwdstr();
	if (!pwd)
		return (MSHELL_FAILURE);
	if (shell_envchr(&exp, lst_envp, "PWD"))
	{
		if (shell_envlst_swp(exp, pwd, envp, lst_envp))
		{
			free(pwd);
			return (MSHELL_FAILURE);
		}
	}
	else
	{
		if (shell_envlst_add(false, pwd, envp, lst_envp))
		{
			free(pwd);
			return (MSHELL_FAILURE);
		}
	}
	free(pwd);
	return (MSHELL_SUCCESS);
}

static int	ps1_init(char ***envp, t_list *lst_envp)
{
	char	*ps1;
	int		exp;

	exp = false;
	ps1 = "PS1=\\u:\\w\\$ ";
	if (shell_envchr(&exp, lst_envp, "PS1"))
	{
		if (shell_envlst_swp(exp, ps1, envp, lst_envp))
			return (MSHELL_FAILURE);
	}
	else
	{
		if (shell_envlst_add(false, ps1, envp, lst_envp))
			return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Initializes core environment variables required by minishell.
 *
 *			Duplicates the original environment into both array and list
 *			formats, then ensures essential variables (PS1, PWD, SHLVL, "_")
 *			are present. Missing entries are created with default values,
 *			while existing ones are synchronized or updated as needed.
 *
 * @param	envp_src	Original environment variable array from main().
 * @param	envp_des	Pointer to store the duplicated environment array.
 * @param	envp_head	Pointer to store the linked list of t_var entries.
 *
 * @note	PS1 is set to "\\u:\\w\\$ " if missing.
 * @note	PWD is updated to match the current working directory.
 * @note	SHLVL defaults to 1 when absent, tracking shell nesting depth.
 * @note	The "_" variable references the last executed command.
 *
 * @return	MSHELL_SUCCESS if initialization completes successfully, or
 *			MSHELL_FAILURE if any setup step fails.
 */
int	shell_envini(char **env_src, char ***env_des, t_list **env_head)
{
	int		exp;
	char	*aux;

	(*env_des) = shell_envdup(env_src);
	if ((*env_des))
		return (MSHELL_FAILURE);
	(*env_head) = shell_envdup_lst(env_src);
	if ((*env_head))
		return (MSHELL_FAILURE);
	shell_sortenv(&(msh->lst_t_var));
	if (ps1_init(env_des, *env_head))
		return (MSHELL_FAILURE);
	if (pwd_init(env_des, *env_head))
		return (MSHELL_FAILURE);
	if (shlvl_init(env_des, *env_head))
		return (MSHELL_FAILURE);
	if (cmdlog_init(env_des, *env_head))
		return (MSHELL_FAILURE);
	shell_sortenv(&(msh->lst_t_var));
	return (MSHELL_SUCCESS);
}
