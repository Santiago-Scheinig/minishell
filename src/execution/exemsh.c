/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exemsh.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:57:49 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 16:31:20 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

static int	get_bicmd(char *cmd)
{
	if (!cmd)
		return (BICMD_NOEXE);
	if (!ft_strncmp(cmd, "export", 7))
		return (BICMD_EXPORT);
	else if (!ft_strncmp(cmd, "cd", 3))
		return (BICMD_CD);
	else if (!ft_strncmp(cmd, "env", 4))
		return (BICMD_ENV);
	else if (!ft_strncmp(cmd, "pwd", 4))
		return (BICMD_PWD);
	else if (!ft_strncmp(cmd, "echo", 5))
		return (BICMD_ECHO);
	else if (!ft_strncmp(cmd, "unset", 6))
		return (BICMD_UNSET);
	else if (!ft_strncmp(cmd, "exit", 5))
		return (BICMD_EXIT);
	else if (ft_strchr(cmd, '='))
		return (BICMD_IMPORT);
	return (BICMD_NOEXE);
}

static int	bicmd(t_bicmd name, t_cmd *exe, t_body *msh)
{
	if (name == BICMD_EXPORT)
	{
		return (msh_export(&(msh->envp), &(msh->envp_lst), &(exe->argv[1])));
		
	}
	else if (name == BICMD_CD)
		return (msh_cd(exe->argv, &(msh->envp_lst)));
	else if (name == BICMD_ENV)
		return (msh_env(exe->argv, msh->envp));
	else if (name == BICMD_PWD)
		return (msh_pwd(exe->argv, msh->envp_lst));
	else if (name == BICMD_ECHO)
		return (msh_echo(exe->argv));
	else if (name == BICMD_UNSET)
		return (msh_unset(&(msh->envp), &(msh->envp_lst), &(exe->argv[1])));
	else if (name == BICMD_EXIT)
		return (msh_exit(exe->argv, msh));
	else if (name == BICMD_IMPORT)
		return (msh_import(&(msh->envp), &(msh->envp_lst), exe->argv));
	return (MSHELL_FAILURE);
}

static int	open_pipe(char *name, int fd, int fileno)
{
	if (dup2(fd, fileno) == -1)
	{
		ft_printfd(2, "msh: %s: Bad file descriptor", name);
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param cmds An array of the comands to execute.
 * @param envp A linked list node containing environment variable data.
 * @return Returns a -1 if there is not execution inside, else exit with 0 if
 * the executed comand works, and 1 or 2 in case an error inside of the
 * commands.
 */
int	child_bicmd(t_cmd *exe, char **envp)
{
	t_body	msh;
	int		num;

	num = get_bicmd(exe->argv[0]);
	if (num)
	{
		ft_memset(&msh, 0, sizeof(t_body));
		msh.envp = envp;
		if (num == BICMD_CD || num == BICMD_EXPORT || num == BICMD_PWD
			|| num == BICMD_UNSET || num == BICMD_IMPORT)
		{
			msh.envp_lst = shell_newlst_var(envp);
			if (!msh.envp_lst)
				exend(MSHELL_FAILURE, "msh: malloc: ", NULL);
		}
		num = bicmd(num, exe, &msh);
		cleanup(&msh);
		exit(num);
	}
	return (MSHELL_FAILURE);
}

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param exe Pointer to the struct of commands.
 * @param minishell Pointer to the main shell structure containing environment
 * variables.
 * @param envp_lst pointer to the struct of exported enviroments.
 * @param envp A linked list node containing environment variable data.
 * @return Returns a -1 if there is not execution inside, 0 if the executed
 * comand works, and 1 or 2 in case an error inside of the commands.
 */
int	father_bicmd(t_cmd *exe, t_body *minishell)
{
	int	num;
	int	i;

	i = 0;
	num = 0;
	if (exe->argv)
		num = get_bicmd(exe->argv[0]);
	if (!num)
		return (MSHELL_FAILURE);
	if (exe->outfd > 2)
	{
		i = dup(STDOUT_FILENO);
		if (open_pipe(exe->argv[0], exe->outfd, STDOUT_FILENO))
			return (MSHELL_FAILURE);
		close(exe->outfd);
		num = bicmd(num, exe, minishell);
		if (open_pipe(exe->argv[0], i, STDOUT_FILENO))
			return (MSHELL_FAILURE);
	}
	else
		num = bicmd(num, exe, minishell);
	return (num);
}
