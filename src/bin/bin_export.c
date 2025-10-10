/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 09:18:19 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_bin.h"
#include "msh_bin_utils.h"

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
 * Builterr
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
			continue;
		envar_name = get_name(argv[i]);
		if (!envar_name)
			return (shell_builterr(SYSFAIL, "export", NULL, 0));
		if (shell_envchr(NULL, envar_name, lst_envp))
			swp = true;
		free(envar_name);
		if (swp && shell_envlst_swp(true, argv[i], envp, lst_envp))
			return (shell_builterr(SYSFAIL, "export", NULL, 0));
		if (!swp && shell_envlst_add(true, argv[i], envp, lst_envp))
			return (shell_builterr(SYSFAIL, "export", NULL, 0));
	}
	return (MSHELL_SUCCESS);
}
