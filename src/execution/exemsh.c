/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exemsh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:57:49 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/23 13:23:13 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

static int	msh_cmd(t_cmd *exe, t_body *msh, t_list *envp_lst, char **envp)
{
	if (!ft_strncmp(exe->argv[0], "export", 7))
		return (msh_export(&envp, &envp_lst, &exe->argv[1]));
	else if (!ft_strncmp(exe->argv[0], "cd", 3))
		return (msh_cd(exe->argv, envp_lst));
	else if (!ft_strncmp(exe->argv[0], "env", 4))
		return (msh_env(exe->argv, envp, envp_lst));
	else if (!ft_strncmp(exe->argv[0], "pwd", 4))
		return (msh_pwd(exe->argv));
	else if (!ft_strncmp(exe->argv[0], "echo", 5))
	{
		msh_echo(exe->argv);
		return (0);
	}
	else if (!ft_strncmp(exe->argv[0], "unset", 6))
		return (msh_unset(envp, envp_lst, &exe->argv[1]));
	else if (!ft_strncmp(exe->argv[0], "exit", 5))
	{
		msh_exit(exe->argv, msh);
		return (1);
	}
	else if (ft_strchr(exe->argv[0], '='))
		return (msh_import(&envp, &envp_lst, exe->argv));
	return (-1);
}

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param minishell Pointer to the main shell structure containing environment
 * variables.
 * @param exe Pointer to the struct of commands.
 * @param envp_lst pointer to the struct of exported enviroments.
 * @param envp A linked list node containing environment variable data.
 * @return Returns a -1 if there is not execution inside, 0 if the executed
 * comand works, and 1 or 2 in case an error inside of the commands.
 * @exception if the command its executed bi a child not the father, the
 * return change for an exit finishing the child.
 *
 * @note This function assumes commands are matched by name and delegates
 * execution accordingly.
 */
int	exe_built(t_cmd *exe, t_body *minishell, t_list *envp_lst, char **envp)
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
		num = msh_cmd(exe, minishell, envp_lst, envp);
		if (dup2(i, STDOUT_FILENO) == -1)
		{
			ft_printfd(2, "msh: %s: Bad file descriptor", exe->argv[0]);
			exit(MSHELL_FAILURE);
		}
	}
	else if (exe->argv)
		num = msh_cmd(exe, minishell, envp_lst, envp);
	return (num);
}
