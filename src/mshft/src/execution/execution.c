/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 16:45:46 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"

static char	**setup_path(const char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (!ft_strncmp("PATH=", envp[i], 5))
			return (ft_split(&envp[i][5], ':'));
	return (NULL);
}

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
			return (shell_redirerr(MSHELL_FAILURE, NULL));
		}
		lst_cmd = lst_cmd->next;
	}
	return (MSHELL_SUCCESS);
}

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
