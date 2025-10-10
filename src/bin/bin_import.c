/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin_import.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 15:44:12 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 09:18:22 by sscheini         ###   ########.fr       */
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

static int	check_arguments(char ***argv)
{
	int	i;
	int	j;

	if (!(*argv))
		return (NULL);
	i = -1;
	while ((*argv)[++i])
	{
		if (!ft_strchr((*argv)[i], '=') || is_valid_var((*argv)[i], 0))
		{
			j = -1;
			while (++j <= i)
				ft_remove_arr((*argv), j);
			return (1);
		}
	}
}

/**
 * Builterr
 */
int	bin_import(char ***argv, char ***envp, t_list *lst_envp)
{
	char	*envar_name;
	int		swp;
	int		exp;
	int		i;

	if (check_arguments(argv))
		return (shell_builterr(INVIDFY, "import", NULL, 0));//No entiendo cual seria este error
	i = -1;
	swp = false;
	while ((*argv)[++i])
	{
		envar_name = get_name((*argv)[i]);
		if (!envar_name)
			return (shell_builterr(SYSFAIL, "import", NULL, 0));
		if (shell_envchr(&exp, envar_name, lst_envp))
			swp = true;
		free(envar_name);
		if (swp && shell_envlst_swp(exp, argv[i], envp, lst_envp))
			return (shell_builterr(SYSFAIL, "import", NULL, 0));
		if (!swp && shell_envlst_add(exp, argv[i], envp, lst_envp))
			return (shell_builterr(SYSFAIL, "import", NULL, 0));
	}
	return (MSHELL_SUCCESS);
}
