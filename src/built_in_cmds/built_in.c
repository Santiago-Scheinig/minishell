/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:13:00 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 19:29:09 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

char	*built_in(t_body *minishell, char *pathname, char **args, t_env *envp)
{
	if (ft_strlen(pathname) == 6 && ft_strnstr(pathname, "export", 6))
		built_export(minishell->envp, envp, args);
	else if (ft_strnstr(pathname, "cd", 2))
		built_cd(minishell, args);
	//else if (ft_strnstr(pathname, "env", ft_strlen(pathname)))
	//	built_env(args);
	else if (ft_strnstr(pathname, "pwd", 3))
		built_pwd(args);
	else if (ft_strnstr(pathname, "echo", 4))
		built_echo(args);
	//else if (ft_strnstr(pathname, "exit", ft_strlen(pathname)))
	//	ft_exit(args);
	else if (ft_strnstr(pathname, "unset", 5))
		built_unset(args, envp);
	return (pathname);
}