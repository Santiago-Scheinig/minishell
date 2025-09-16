/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:52:58 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/16 14:26:43 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

int	built_end(char *name, char *type, char *flags, char error)
{
	char	*shell;

	shell = "minishell: ";
	if (ft_strnstr(type, "Numbers of args", ft_strlen(type)))
		ft_printfd(2, "%s%s: too many arguments\n", shell, name);
	else if (ft_strnstr(type, "Invalid flags", ft_strlen(type)))
	{
		if (error)
			ft_printfd(2, "%s%s: -%c: invalid option\n", shell, name, error);
		else
			ft_printfd(2, "%s%s: -: invalid option\n", shell, name);
		ft_printfd(2, "%s: usage: %s %s\n", name, name, flags);
		return (2);
	}
	else if (ft_strnstr(type, "HOME", ft_strlen(type)))
		ft_printfd(2, "%s%s: HOME not set\n", shell, name);
	else if (ft_strnstr(type, "System failed", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s\n", shell, name, strerror(errno));
	else if (ft_strnstr(type, "Not valid identifier", ft_strlen(type)))
		ft_printfd(2, "%s%s: `%s': not a valid identifier\n", shell, name,
			flags);
	else if (ft_strnstr(type, "Numeric arg required", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s: numeric argument required\n", shell, name,
			flags);
	return (1);
}

char	*built_in(t_body *minishell, char *pathname, char **args, t_list *lst)
{
	t_var	*envp;

	envp = (t_var *)lst->content;
	if (ft_strlen(pathname) == 6 && ft_strnstr(pathname, "export", 6))
		b_export(&minishell->envp, lst, &args[1]);
	else if (ft_strnstr(pathname, "cd", 2))
		cd(args, lst);
	else if (ft_strnstr(pathname, "env", ft_strlen(pathname)))
		env(args, &minishell->envp[0]);
	else if (ft_strnstr(pathname, "pwd", 3))
		pwd(args);
	else if (ft_strnstr(pathname, "echo", 4))
		echo(args);
	else if (ft_strnstr(pathname, "exit", ft_strlen(pathname)))
		b_exit(args, minishell);
	else if (ft_strnstr(pathname, "unset", 5))
		unset(minishell->envp, lst, &args[1]);
	return (pathname);
}
