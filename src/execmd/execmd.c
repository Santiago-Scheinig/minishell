/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:14 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/26 18:23:26 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execmd.h"

/**
 * Executes and pipes every command specified on the program enviroment,
 * but the first command will read from the STDIN instead of the infile 
 * (until the LIMITATOR is writen).
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
void	exe_heredoc(t_cmd *exe, t_body *minishell)
{
	int	i;
	int	infd;
	int	pipefd[2];

	env->waitpid_list = ft_calloc((env->cmd_count), sizeof(pid_t));
	if (!env->waitpid_list)
		sigend(minishell, 1);
	while (++i < env->cmd_count)
	{
		if (!i)
			infd = ft_read_to_limitator(limitator);
		if (infd < 0 || pipe(pipefd) == -1)
			return (ft_waitfor_childs(env, EXIT_FAILURE));
		if (i == env->cmd_count - 1)
		{
			close(pipefd[1]);
			pipefd[1] = open(env->outfile, O_CREAT | O_APPEND | O_WRONLY, 420);
			if (pipefd[1] < 0)
				return (ft_waitfor_childs(env, EXIT_FAILURE));
		}
		infd = ft_do_fork(env, infd, pipefd, envp);
	}
	return (ft_waitfor_childs(env, EXIT_SUCCESS));
}

/**
 * Executes and pipes every command specified on the program enviroment.
 * @param env The main enviroment pipex structure.
 * @param envp The main enviroment path.
 * @return Returns 0 on a successful execution and -1 in case of error.
 * @note Regardless of the return, it will wait for any child process
 * created during the command executions.
 */
int	exe_child(t_pipex *env, char **envp)
{
	int	i;
	int	infd;
	int	pipefd[2];

	i = -1;
	infd = -1;
	env->waitpid_list = ft_calloc((env->cmd_count), sizeof(pid_t));
	if (!env->waitpid_list)
		return (-1);
	while (++i < env->cmd_count)
	{
		if (!i && env->infile)
			infd = open(env->infile, O_RDONLY);
		if (pipe(pipefd) == -1)
			return (ft_waitfor_childs(env, EXIT_FAILURE));
		if (i == env->cmd_count - 1)
		{
			close(pipefd[1]);
			pipefd[1] = open(env->outfile, O_CREAT | O_TRUNC | O_WRONLY, 0644);
			if (pipefd[1] < 0)
				return (ft_waitfor_childs(env, EXIT_FAILURE));
		}
		infd = ft_do_fork(env, infd, pipefd, envp);
	}
	return (ft_waitfor_childs(env, EXIT_SUCCESS));
}

static char	*cmd_verification(t_cmd *exe, char **path_env)
{
	char	*pathname;

	if (exe->infile < 0)
	{
		if (exe->infile != -2 || !exe->limitator)
			return (NULL);
		else if (exe->heredoc[0] < 0 || exe->heredoc[1] < 0)
			return (NULL);
	}
	else if (exe->outfile < 0)
		return (NULL);
	pathname = cmd_pathname(exe->argv[0], path_env);
	if (!pathname)
	{
		strerror(0);
		return (NULL);
	}
	return (pathname);
}

/**
 * Pipex bonus works in the same way than the original, adding the following
 * extra functionalities:
 * 
 *	- Manage multiple pipes.
 *	- Accepts << and >> operators, when here\_doc is specified on the main
 *	argument as argv[1]; 
 */
int	execmd(t_body *minishell)
{
	t_list	*cmd_lst;
	t_cmd	*exe;
	char	**path;

	cmd_lst = minishell->cmd_lst;
	while (cmd_lst)
	{
		exe = (t_cmd *) cmd_lst->content;
		path = ft_check_path(minishell->envp);
		if (!path)
			sigend(minishell, 1);
		exe->pathname = cmd_verification(exe, minishell);
		if(exe->pathname) 
		{
			if (exe->built_in == 1 && !minishell->cmd_lst->next)//means theres only one
				//exe_bicmd();
			else if (exe->infile == -2 && exe->limitator)
				exe_heredoc(exe, minishell);
			else
				exe_child(env, envp);
		}
		cmd_lst = cmd_lst->next;
	}
	ft_split_free(path);
	ft_waitfor_childs(minishell, minishell->errno);
}
