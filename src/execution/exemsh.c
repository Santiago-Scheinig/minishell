/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exemsh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:57:49 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/21 17:22:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param minishell Pointer to the main shell structure containing environment
 * variables.
 * @param pathname The name of the built-in command to execute.
 * @param args The arguments passed to the command.
 * @param lst A linked list node containing environment variable data.
 * @return Returns the pathname of the executed built-in command.
 *
 * @note This function assumes commands are matched by name and delegates
 * execution accordingly.
 */
int	msh_cmd(t_cmd *exe, t_body *minishell)
{
	if (!ft_strncmp(exe->argv[0], "export", 7))
		return (msh_export(&minishell->envp, &minishell->envp_lst, &exe->argv[1]));
	else if (!ft_strncmp(exe->argv[0], "cd", 3))
		return (msh_cd(exe->argv, minishell->envp_lst));
	else if (!ft_strncmp(exe->argv[0], "env", 4))
		return (msh_env(exe->argv, minishell->envp, minishell->envp_lst));
	else if (!ft_strncmp(exe->argv[0], "pwd", 4))
		return (msh_pwd(exe->argv));
	else if (!ft_strncmp(exe->argv[0], "echo", 5))
	{
		msh_echo(exe->argv);
		return (0);
	}
	else if (!ft_strncmp(exe->argv[0], "unset", 6))
		return (msh_unset(minishell->envp, minishell->envp_lst, &exe->argv[1]));
	else if (!ft_strncmp(exe->argv[0], "exit", 5))
	{
		msh_exit(exe->argv, minishell);
		return (1);
	}
	else if (ft_strchr(exe->argv[0], '='))
		return (msh_import(&minishell->envp, &minishell->envp_lst, exe->argv));
	return (-1);
}

int	exe_built(t_cmd *exe, t_body *minishell)
{
	int	num;
	int	i;

	i = 0;
	num = -1;
	if (exe->argv && exe->outfd > 2)
	{
		i = dup(STDOUT_FILENO);
		if (dup2(exe->outfd, STDOUT_FILENO) == -1)
		{
			ft_printfd(2, "msh: %s: Bad file descriptor", exe->argv[0]);
			exit(MSHELL_FAILURE);
		}
		close(exe->outfd);
		num = msh_cmd(exe, minishell);
		if (dup2(i, STDOUT_FILENO) == -1)
		{
			ft_printfd(2, "msh: %s: Bad file descriptor", exe->argv[0]);
			exit(MSHELL_FAILURE);
		}
	}
	else if (exe->argv)
		num = msh_cmd(exe, minishell);
	return (num);
}
