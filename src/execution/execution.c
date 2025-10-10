/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 11:03:55 by sscheini         ###   ########.fr       */
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

static int	setup_pipeline(t_list *cmd_lst)
{
	t_list	*aux;
	t_cmd	*exe;
	t_cmd	*exe_next;
	int		i;

	i = -1;
	aux = cmd_lst;
	while (cmd_lst)
	{
		exe = cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = cmd_lst->next->content;
		if (fd_setexe(exe, exe_next))
		{
			fd_endexe(aux, 1);
			return (redirend(NULL, MSHELL_FAILURE));
		}
		cmd_lst = cmd_lst->next;
	}
	return (MSHELL_SUCCESS);
}

static int	setup_waitexec(t_body *msh)
{
	int	cmd_len;

	cmd_len = ft_lstsize(msh->lst_t_cmd);
	msh->childs_pid = ft_calloc(cmd_len, sizeof(pid_t));
	if (!msh->childs_pid)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	msh->err_fd = ft_calloc(cmd_len, sizeof(int));
	if (!msh->err_fd)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
}

static char	**setup(t_body *msh)
{
	char	**path;

	setup_waitexec(msh);
	if (setup_pipeline(msh->lst_t_cmd))
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
	if (msh->interactive)
		shell_sigign_all();
	return (path);
}

int	execution(t_body *msh)
{
	t_cmd	*exe;
	char	**path;
	int		bin_no;

	if (!msh->head_cmd->next)
	{
		exe = (t_cmd *) lst_cmd->content;
		if (!exe->argv)
			return (MSHELL_FAILURE);
		bin_no = getbin(exe->argv[0]);
		if (bin_no)
			return (exebin_parent(bin_no, exe, msh));
	}
	path = setup(msh);
	if (!path)
		return (MSHELL_FAILURE);	
	return (execmd_child(msh->head_cmd, path, msh));
}
