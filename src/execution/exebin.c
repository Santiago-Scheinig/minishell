/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exebin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:57:49 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 10:23:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

static int	exebin(int bin_no, t_cmd *exe, t_body *msh)
{
	if (bin_no == BINCMD_EXPORT)
		return (bin_export(&(exe->argv[1]), &(msh->envp), msh->lst_t_var));
	else if (bin_no == BINCMD_CD)
		return (bin_cd(exe->argv, &(msh->lst_t_var)));
	else if (bin_no == BINCMD_ENV)
		return (bin_env(exe->argv, msh->envp));
	else if (bin_no == BINCMD_PWD)
		return (bin_pwd(exe->argv, msh->lst_t_var));
	else if (bin_no == BINCMD_ECHO)
		return (bin_echo(exe->argv));
	else if (bin_no == BINCMD_UNSET)
		return (bin_unset(&(exe->argv[1]), &(msh->envp), &(msh->lst_t_var)));
	else if (bin_no == BINCMD_EXIT)
		return (bin_exit(exe->argv, msh));
	else if (bin_no == BINCMD_IMPORT)
		return (bin_import(exe->argv, &(msh->envp), &(msh->lst_t_var)));
	return (MSHELL_FAILURE);
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
int	exebin_child(t_cmd *exe, char **envp)
{
	t_body	msh;
	int		bin_no;
	int		exit_no;

	bin_no = getbin(exe->argv[0]);
	if (bin_no == BINCMD_NOEXE)
		return (MSHELL_FAILURE);
	ft_memset(&msh, 0, sizeof(t_body));
	msh.envp = envp;
	if (bin_no == BINCMD_CD || bin_no == BINCMD_EXPORT
		|| bin_no == BINCMD_PWD || bin_no == BINCMD_UNSET
		|| bin_no == BINCMD_IMPORT)
	{
		msh.head_envar = shell_envdup_lst(envp);
		if (!msh.head_envar)
			exend(MSHELL_FAILURE, "msh: malloc: ", NULL);
	}
	exit_no = exebin(bin_no, exe, &msh);
	shell_cleanup(true, &msh);
	exit(exit_no);
}

//As a father, i should dup2 the STDERR also to a pipeerr, so when execution finishes
//i can print the error correctly, the true issue is to terminate the program
int	exebin_parent(int bin_no, t_cmd *exe, t_body *msh)
{
	int	exit_no;
	int	orig_stdout;

	orig_stdout = -1;
	if (exe->outfd > 2)
	{
		orig_stdout = dup(STDOUT_FILENO);
		if (dup2(exe->outfd, STDOUT_FILENO) == -1)
			return (shell_redirerr(MSHELL_FAILURE, exe->argv[0]));
		close(exe->outfd);
	}
	exit_no = exebin(bin_no, exe, msh);
	if (exit_no == MSHELL_FATAL)//-1 malloc error, not sure how to make it happen yet
		shell_forcend(MSHELL_FAILURE, "malloc", MSHELL_FAILURE);
	if (orig_stdout > -1)
		if (dup2(STDOUT_FILENO, orig_stdout))
			return (shell_redirerr(MSHELL_FAILURE, exe->argv[0]));
	return (MSHELL_SUCCESS);
}
