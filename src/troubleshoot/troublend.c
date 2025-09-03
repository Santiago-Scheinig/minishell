/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   troublend.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/03 21:58:59 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "troubleshoot.h"

void	cleanup(t_body *minishell)
{
	if (minishell->childs_pid)
	{
		free(minishell->childs_pid);
		minishell->childs_pid = NULL;
	}
	if (minishell->input)
	{
		free(minishell->input);
		minishell->input = NULL;
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

// int	childend()

int	redirend(char *argv, t_error number)
{
	if (number)
	{
		if (!argv)
			argv = "pipe";
		write(2, "msh: ", 5);
		perror(argv);
	}
	return (number);
}

int	sigend(const char *next, t_error number, t_body *minishell)
{
	const char	*msg = "syntax error near unexpected token";
	int			line;

	line = minishell->line;
	if (number == MSHELL_MISSUSE)
	{
		if (!next)
			next = "newline";
		if (minishell->interactive)
			ft_printfd(2, "msh: %s \'%s\'\n", msg, next);
		else
			ft_printfd(2, "msh: line %i: %s \'%s\'\n", line, msg, next);
	}
	g_signal_received = SIGUSR1;
	return (number);
}

int	forcend(t_body *minishell, char *function, t_error number)
{
	cleanup(minishell);
	if (minishell->envp_lst)
		shell_lstclear(&minishell->envp_lst, shell_lstdelvar);
	if (minishell->envp)
		ft_split_free(minishell->envp);
	if (minishell->interactive)
		rl_clear_history();
	if (number == MSHELL_SUCCESS && minishell->interactive)
		exit(MSHELL_SUCCESS);
	if (number == MSHELL_FAILURE && function)
		ft_printfd(2, "\n");
	if (number == MSHELL_CMD_NOTEXE && function)
		ft_printfd(2, "minishell: %s: No such file or directory\n", function);
	if (number != MSHELL_FATAL 
		&& tcsetattr(STDIN_FILENO, TCSANOW, &minishell->orig_term))
		exit(MSHELL_FAILURE);
	exit(number);
}
