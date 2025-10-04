/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_pmtstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 13:51:35 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 16:47:32 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"

static char	*shell_pwd(void)
{
	char	*pwd;
	char	*aux;

	aux = getcwd(NULL, 0);
	pwd = ft_strjoin("PWD=", aux);
	free(aux);
	return (pwd);
}



static char	*shell_lastcmd(t_list *envp)
{
	char	*last_cmd;
	char	*aux;

	aux = shell_getenv(envp, "_");
	if (!aux)
		last_cmd = ft_strjoin("_=", "./minishell");
	else
		last_cmd = ft_strjoin("_=", aux);
	return (last_cmd);
}

static char	*shell_ps1(t_list *envp)
{
	t_var	*tmp;
	char	*ps1;

	while (envp && envp->content)
	{
		tmp = (t_var *)envp->content;
		if (!ft_strncmp(tmp->name, "PS1", 3))
		{
			free(tmp->value);
			ps1 = ft_strdup("\\u:\\w\\$ ");
			if (!ps1)
				return (NULL);
			tmp->value = ps1;
			return (ps1);
		}
		envp = envp->next;
	}
	ps1 = ft_strjoin("PS1=", "\\u:\\w\\$ ");
	return (ps1);
}

/**
 * Builds an array of strings used to initialize environment-related defaults.
 * 
 * @param envp Linked list of environment variables (t_var nodes).
 * 
 * Constructs and returns a newly allocated NULL-terminated array of strings
 * containing default or updated entries for PS1, PWD, _ and SHLVL.
 * 
 * @return Pointer to a newly allocated array of strings (terminated by NULL),
 *         or NULL on allocation failure.
 * @note - Caller is responsible for freeing each string and the array.
 * @note - On allocation failure this function returns NULL and may leave
 *         partial state in envp's list (PS1 may have been modified).
 */
char	**shell_pmtstr(t_list *envp)
{
	char	**ps;

	ps = malloc(5 * sizeof(char *));
	if (!ps)
		return (NULL);
	ps[0] = shell_ps1(envp);
	if (!ps[0])
		return (NULL);
	ps[1] = shell_pwd();
	if (!ps[1])
		return (NULL);
	ps[2] = shell_lastcmd(envp);
	if (!ps[2])
		return (NULL);
	ps[3] = ft_strjoin("SHLVL=", "1");
	if (!ps[3])
		return (NULL);
	ps[4] = NULL;
	return (ps);
}
