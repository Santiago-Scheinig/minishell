/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/12/15 12:21:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

/**
 * @brief	Extracts and splits the PATH variable from the environment.
 *
 *			Searches envp for the variable "PATH=" and splits its
 *			value into an array of directory strings using ':' as
 *			delimiter.
 *
 * @param	envp	Environment array.
 *
 * @return	Array of PATH directories, or NULL if PATH not found.
 */
static char	**setup_path(const char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp("PATH=", envp[i], 5))
			return (ft_split(&envp[i][5], ':'));
	return (NULL);
}

/**
 * @brief	Configures file descriptors for all commands in the pipeline.
 *
 *			Iterates through lst_cmd and calls exe_setfd() for each
 *			command, connecting stdout of one to stdin of the next.
 *
 * @param	lst_cmd	Linked list of commands to set up.
 *
 * @return	MSHELL_SUCCESS on success, or error code if redirection fails.
 */
static int	setup_pipeline(t_list *lst_cmd)
{
	t_list	*aux;
	t_cmd	*exe;
	t_cmd	*exe_next;
	int		i;

	i = -1;
	aux = lst_cmd;
	while (lst_cmd)
	{
		exe = lst_cmd->content;
		exe_next = NULL;
		if (lst_cmd->next)
			exe_next = lst_cmd->next->content;
		if (exe_setfd(exe, exe_next))
		{
			exe_endfd(aux, 1);
			return (shell_redirerr(MSHELL_FAILURE, NULL, 0));
		}
		lst_cmd = lst_cmd->next;
	}
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Prepares the shell structure for command execution.
 *
 *			Allocates msh->childs_pid and msh->err_fd arrays based
 *			on the number of commands to be executed.
 *
 * @param	msh		Shell context to initialize.
 */
static void	setup_waitexec(t_body *msh)
{
	int	cmd_len;

	cmd_len = ft_lstsize(msh->head_cmd);
	msh->childs_pid = ft_calloc(cmd_len, sizeof(pid_t));
	if (!msh->childs_pid)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	msh->err_fd = ft_calloc(cmd_len, sizeof(int));
	if (!msh->err_fd)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	ft_memset(msh->err_fd, -1, cmd_len * sizeof(int));
}

/**
 * @brief	Performs all setup required before executing commands.
 *
 *			Calls setup_waitexec(), setup_pipeline(), and
 *			setup_path(). Handles allocation errors and ensures
 *			PATH array exists.
 *
 * @param	msh		Shell context to prepare.
 *
 * @return	Array of PATH directories for command search, or NULL on failure.
 */
static char	**setup(t_body *msh)
{
	char	**path;

	setup_waitexec(msh);
	if (setup_pipeline(msh->head_cmd))
	{
		if (errno == ENOMEM)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		msh->exit_no = MSHELL_FAILURE;
		return (NULL);
	}
	path = setup_path((const char **) msh->envp);
	if (!path)
	{
		if (errno != ENOMEM)
			path = ft_calloc(1, sizeof(char *));
		if (!path)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		return (path);
	}
	return (path);
}

/**
 * @brief	Main execution function for the minishell.
 *
 *			Handles single built-in commands via exebin_parent(),
 *			or multiple commands via execmd_child(). Sets up signal
 *			handling and PATH array.
 *
 * @param	msh		Shell context with commands, environment, and state.
 *
 * @note	Handles interactive mode signals and cleans up on errors.
 *
 * @return	Exit code of the executed commands, or MSHELL_FAILURE on error.
 */
int	execution(t_body *msh)
{
	t_cmd	*exe;
	char	**path;
	int		exit_no;
	int		bin_no;

	if (!msh->head_cmd->next)
	{
		exe = (t_cmd *) msh->head_cmd->content;
		if (!exe->argv)
			return (MSHELL_FAILURE);
		bin_no = getbin(exe->argv[0]);
		if (bin_no)
			return (exebin_parent(bin_no, exe, msh));
	}
	if (msh->interactive)
		shell_sigign_all();
	path = setup(msh);
	if (!path)
		return (MSHELL_FAILURE);
	exit_no = execmd_child(path, msh->head_cmd, msh);
	if (msh->interactive)
		if (shell_sigint(msh->interactive) || shell_sigquit())
			shell_forcend(MSHELL_FAILURE, "sigaction", msh);
	return (exit_no);
}
