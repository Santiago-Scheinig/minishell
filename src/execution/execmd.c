/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/20 17:43:30 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

/**
 * Creates and allocates a STRING with the definitive path to a cmd binary.
 * 
 * @param cmd The name of the command binary to find.
 * @param path The enviroment path where to search the command binary.
 * @return A pointer to the new STRING or NULL if the allocation failed or
 * the cmd can't be access or found as binary on path.
 */
static int exe_getpath(char *cmd, char **path, char **pathname)
{
	char	*tmp;
	int		i;

	i = -1;
	if (ft_strchr(cmd, '/'))
	{
		(*pathname) = cmd;
		if (!access(cmd, X_OK))
			return (MSHELL_SUCCESS);
		return (MSHELL_CMD_INVEXE);//print no access error!
	}
	while (path[++i] && cmd)
	{
		tmp = ft_strjoin(path[i], "/");
		if (!tmp)
			return (MSHELL_FAILURE);//this are malloc error... i need to end the program
		(*pathname) = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!(*pathname))
			return (MSHELL_FAILURE);//this are malloc error... i need to end the program
		if (!access((*pathname), X_OK))
			return (MSHELL_SUCCESS);
		free((*pathname));
	}
	return (MSHELL_CMD_INVEXE);//print command not found
}

static char	**exe_setup(t_body *minishell)
{
	char	**path;
	int		cmd_len;
	
	cmd_len = ft_lstsize(minishell->cmd_lst);
	minishell->childs_pid = calloc(cmd_len, sizeof(pid_t));
	if (!minishell->childs_pid)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	if (setup_pipeline(minishell->cmd_lst))
		return (NULL);
	path = setup_path((const char **) minishell->envp);
	if (!path)
	{
		path = calloc(1, sizeof(char *));
		if (!path)
			forcend(minishell, "malloc", MSHELL_FAILURE);
		return (path);
	}
	return (path);
}

static void	exe_child(t_cmd *exe, char **path, pid_t *child, char **envp)
{
	int error;

	(*child) = fork();
	if (!(*child) && !(exe->fd.exein < 0))
	{
		if (sigdfl())
			exit(MSHELL_FAILURE);//childend();
		if (!exe->argv || !exe->argv[0])
			exit (MSHELL_FAILURE);//redir error
		error = exe_getpath(exe->argv[0], path, &(exe->pathname));//should print all cmd related errors
		if (error)
			exit (error);//should return the error number asosiated with cmd errors
		if (dup2(exe->fd.exein, STDIN_FILENO) == -1
			|| dup2(exe->fd.exeout, STDOUT_FILENO) == -1)
			exit(MSHELL_FAILURE);//childend();
		fd_endexe(exe);
		//if (exe_built())
		// {
		if (execve(exe->pathname, exe->argv, envp))
			exit(MSHELL_FAILURE);//childend();
		// }
	}
	fd_endexe(exe);
}

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
/* static int	exe_built(t_cmd *exe, t_body *minishell)//No minishell sadly
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
		msh_echo(exe->argv);
	else if (!ft_strncmp(exe->argv[0], "unset", 6))
		return (msh_unset(minishell->envp, minishell->envp_lst, &exe->argv[1]));
	else if (!ft_strncmp(exe->argv[0], "exit", 5))
	{
		msh_exit(exe->argv, minishell);
		return (1);
	}
	else
		return (msh_import(&minishell->envp, &minishell->envp_lst, exe->argv));
	return (0);
} */

int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	t_cmd	*exe_next;
	char	**path;
	int	i;

	cmd_lst = minishell->cmd_lst;
	// if (!cmd_lst->next)
	// 	exe_built((t_cmd *) minishell->cmd_lst->content, minishell);
	path = exe_setup(minishell);
	if (!path)
		return (MSHELL_FAILURE);//set errno to 1 also!
	i = -1;
	while (cmd_lst)
	{
		exe = (t_cmd *) cmd_lst->content;
		exe_next = NULL;
		if (cmd_lst->next)
			exe_next = (t_cmd *) cmd_lst->next->content;
		exe_child(exe, path, &(minishell->childs_pid[++i]), minishell->envp);
		cmd_lst = cmd_lst->next;
	}
	ft_split_free(path);
	return (MSHELL_SUCCESS);
}
