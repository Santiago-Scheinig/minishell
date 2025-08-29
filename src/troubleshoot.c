/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   troubleshoot.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/29 17:44:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->current_next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	cleanup(t_body *minishell)
{
	if (minishell->input)
	{
		free(minishell->input);
		minishell->input = NULL;
	}
	if (minishell->envp)
	{
		ft_split_free(minishell->envp);
		minishell->envp = NULL;
	}
	if (minishell->lst_env)
	{
		shell_lstclear(&(minishell->lst_env), shell_lstdelenv);
		minishell->lst_env = NULL;
	}
	if (minishell->token_lst)
	{
		shell_lstclear(&(minishell->token_lst), shell_lstdeltkn);
		minishell->token_lst = NULL;
	}
	if (minishell->cmd_lst)
	{
		shell_lstclear(&(minishell->cmd_lst), shell_lstdelcmd);
		minishell->cmd_lst = NULL;
	}
}

int	sigend(const char *next, t_error number)
{
	if (number == MSHELL_MISSUSE)
	{
		if (!next)
			next = "\n";
		printf("minishell: parse error near \'%s\'\n", next);
	}
	kill(0, SIGUSR1);
	return (number);
}

int	forcend(t_body *minishell, char *function, t_error number)
{
	cleanup(minishell);
	if (number == MSHELL_SUCCESS && minishell->interactive)
		//bi_exit(MSHELL_SUCCESS);
	if (number == MSHELL_FAILURE && function)
		printf("\n");
	if (number == MSHELL_CMD_NOTEXE && function)
		printf("minishell: %s: No such file or directory\n", function);
	if (number != MSHELL_FATAL 
		&& tcsetattr(STDIN_FILENO, TCSANOW, &minishell->orig_term))
	{
		//perror();
		exit(MSHELL_FAILURE);
	}
	exit(number);
}