/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   troubleshoot.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/16 13:44:59 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "troubleshoot.h"

void	cleanup(t_body *minishell)
{
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

int	sigend(t_error number)
{
	if (number == MSHELL_MISSUSE)
		printf("Invalid input\n");
	kill(0, SIGUSR1);
	return (number);
}

int	forcend(t_body *minishell, char *function, t_error number)
{
	cleanup(minishell);
	if (number == MSHELL_SUCCESS && minishell->interactive)
		exit(MSHELL_SUCCESS);
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
