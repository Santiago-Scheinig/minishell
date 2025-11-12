/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exebin.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 14:57:49 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/12 17:30:10 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_bin.h"

/**
 * @brief	Executes the corresponding built-in shell command.
 *
 *			Dispatches execution based on 'bin_no' to built-in
 *			functions like export, cd, env, pwd, echo, unset,
 *			exit, and import.
 *
 * @param	bin_no	Identifier of the built-in command.
 * @param	exe		Command structure containing arguments.
 * @param	msh		Shell context containing environment and state.
 *
 * @return	Exit code returned by the executed command, or
 *			MSHELL_FAILURE if the command is invalid.
 */
static int	exebin(int bin_no, t_cmd *exe, t_body *msh)
{
	if (bin_no == BINCMD_EXPORT)
		return (bin_export(&(exe->argv[1]), &(msh->envp), msh->head_envar));
	else if (bin_no == BINCMD_CD)
		return (bin_cd(exe->argv, &(msh->head_envar)));
	else if (bin_no == BINCMD_ENV)
		return (bin_env(exe->argv, msh->envp));
	else if (bin_no == BINCMD_PWD)
		return (bin_pwd(exe->argv, msh->head_envar));
	else if (bin_no == BINCMD_ECHO)
		return (bin_echo(exe->argv));
	else if (bin_no == BINCMD_UNSET)
		return (bin_unset(&(exe->argv[1]), &(msh->envp), &(msh->head_envar)));
	else if (bin_no == BINCMD_EXIT)
		return (bin_exit(exe->argv, msh));
	else if (bin_no == BINCMD_IMPORT)
		return (bin_import(&(exe->argv), &(msh->envp), msh->head_envar));
	return (MSHELL_FAILURE);
}

/**
 * @brief	Restores standard output and error after command execution.
 *
 *			Uses saved file descriptors in 'fd' to restore STDOUT
 *			and STDERR. Sends any error messages read from 'errfd_read'.
 *
 * @param	exit_no	Exit code from executed command.
 * @param	fd		Original file descriptors for STDOUT/STDERR.
 * @param	errfd_read	File descriptor to read error messages.
 * @param	msh		Shell context for interactive mode and line info.
 *
 * @return	0 on success, or error code from shell_redirerr().
 */
static int	exebin_end(int exit_no, t_orig fd, int errfd_read, t_body *msh)
{
	if (fd.orig_stdout > -1)
	{
		if (dup2(fd.orig_stdout, STDOUT_FILENO) == -1)
			return (shell_redirerr(MSHELL_FATAL, "STDOUT_FILENO:"));
		close(fd.orig_stdout);
	}
	if (fd.orig_stderr > -1)
	{
		if (dup2(fd.orig_stderr, STDERR_FILENO) == -1)
			return (shell_redirerr(MSHELL_FATAL, "STDERR_FILENO:"));
		close(fd.orig_stderr);
	}
	return (err_msgfd(exit_no, errfd_read, msh->interactive, msh->line));
}

/**
 * @brief	Prepares file descriptors for command execution.
 *
 *			Saves original STDOUT/STDERR, redirects output if necessary,
 *			and sets up error redirection for child execution.
 *
 * @param	fd				Pointer to t_orig struct storing original fds.
 * @param	errfd_write		Error redirection write descriptor.
 * @param	exe				Command structure containing arguments and fds.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on redirection error.
 */
static int	exebin_ini(t_orig *fd, int *errfd_write, t_cmd *exe)
{
	fd->orig_stdout = -1;
	fd->orig_stderr = dup(STDERR_FILENO);
	if (exe->outfd > 2)
	{
		fd->orig_stdout = dup(STDOUT_FILENO);
		if (dup2(exe->outfd, STDOUT_FILENO) == -1)
			return (shell_redirerr(MSHELL_FAILURE, exe->argv[0]));
		close(exe->outfd);
	}
	if ((*errfd_write) != -1 && dup2((*errfd_write), STDERR_FILENO) == -1)
		shell_redirerr(MSHELL_FAILURE, exe->argv[0]);
	if ((*errfd_write) != -1)
		close((*errfd_write));
	(*errfd_write) = -1;
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Executes a built-in command in a child process.
 *
 *			Handles environment duplication if needed, calls the
 *			corresponding built-in via exebin(), and exits with
 *			the command's exit code.
 *
 * @param	exe		Command to execute with arguments.
 * @param	envp	Environment array.
 *
 * @note	Memory cleanup is handled via shell_cleanup().
 * @note	Exits the process with the proper exit code.
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
			err_endexe(MSHELL_FAILURE, "malloc: ", NULL);
	}
	exit_no = exebin(bin_no, exe, &msh);
	shell_cleanup(true, &msh);
	exit(exit_no);
}

/**
 * @brief	Executes a built-in command in the parent process.
 *
 *			Handles redirection initialization, calls exebin(),
 *			restores original fds, and updates msh->exit_no.
 *
 * @param	bin_no	Identifier of the built-in command.
 * @param	exe		Command structure containing arguments.
 * @param	msh		Shell context containing environment and state.
 *
 * @note	Handles memory allocation errors and forced cleanup.
 *
 * @return	Exit code from the executed command.
 */
int	exebin_parent(int bin_no, t_cmd *exe, t_body *msh)
{
	t_orig	fd;
	int		errfd[2];
	int		exit_no;

	ft_memset(&fd, -1, sizeof(t_orig));
	ft_memset(errfd, -1, 2 * sizeof(int));
	if (pipe(errfd) == -1)
		shell_redirerr(MSHELL_FAILURE, NULL);
	if (exebin_ini(&fd, &errfd[1], exe))
		return (MSHELL_FAILURE);
	exit_no = exebin(bin_no, exe, msh);
	if (exit_no == MSHELL_FAILURE)
	{
		if (errno == ENOMEM)
		{
			exebin_end(exit_no, fd, errfd[0], msh);
			shell_forcend(MSHELL_FAILURE, NULL, msh);
		}
	}
	if (exebin_end(exit_no, fd, errfd[0], msh))
		shell_forcend(MSHELL_FAILURE, NULL, msh);
	msh->exit_no = exit_no;
	return (exit_no);
}
