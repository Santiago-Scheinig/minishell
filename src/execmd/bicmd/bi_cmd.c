/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:52:58 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/06 20:04:00 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bicmd.h"

char	*built_in(t_body *minishell, char *pathname, char **args, t_list *lst)
{
	t_var	*envp;
	
	sortenv(&minishell->envp_lst);
	envp = (t_var *)lst->content;
	if (ft_strlen(pathname) == 6 && ft_strnstr(pathname, "export", 6))
		b_export(&minishell->envp, lst, &args[1]);
	else if (ft_strnstr(pathname, "cd", 2))
		cd(args, lst);
	//else if (ft_strnstr(pathname, "env", ft_strlen(pathname)))
	//	built_env(args);
	else if (ft_strnstr(pathname, "pwd", 3))
		pwd(args);
	else if (ft_strnstr(pathname, "echo", 4))
		echo(args);
	//else if (ft_strnstr(pathname, "exit", ft_strlen(pathname)))
	//	ft_exit(args);
	else if (ft_strnstr(pathname, "unset", 5))
		unset(minishell->envp, lst, &args[1]);
	return (pathname);
}
