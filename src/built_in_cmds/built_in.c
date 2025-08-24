/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:13:00 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/24 17:29:32 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

char	*built_in(t_body *minishell, char *pathname, char **args, t_env *envp)
{
	if (ft_strnstr(pathname, "export", ft_strlen(pathname)))
		built_export(minishell->envp, envp, args);
	else if (ft_strnstr(pathname, "cd", ft_strlen(pathname)))
		built_cd(minishell, args);
	//else if (ft_strnstr(pathname, "env", ft_strlen(pathname)))
	//	built_env(args);
	else if (ft_strnstr(pathname, "pwd", ft_strlen(pathname)))
		built_pwd(args);
	else if (ft_strnstr(pathname, "echo", ft_strlen(pathname)))
		built_echo(args);
	//else if (ft_strnstr(pathname, "exit", ft_strlen(pathname)))
	//	ft_exit(args);
	else if (ft_strnstr(pathname, "unset", ft_strlen(pathname)))
		built_unset(args, envp);
	return (pathname);
}