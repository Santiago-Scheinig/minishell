/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   troublend.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/21 13:15:24 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib/msh_std.h"
#include "lib/msh_tsh.h"
#include <readline/readline.h>
#include <readline/history.h>

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

/**
 * Handles error messages for built-in shell commands and print stderr.
 * Depending on the error type and flags, it formats and outputs the
 * appropriate message.
 * 
 * @param name The name of the command that caused the error.
 * @param type A string indicating the type of error to handle.
 * @param flags Additional flags or arguments related to the error.
 * @param error A character representing a specific invalid flag option
 * @return Returns 2 for invalid flag errors, otherwise returns 1.
 * 
 * @note Prints error messages prefixed with "minishell:".
 * @note Supports various error types such as argument count, Invalid flags,
 * missing HOME, System errors, Invalid identifiers, and Numeric argument
 * errors.
 */
int	built_end(char *name, char *type, char *flags, char error)
{
	char	*shell;

	shell = "msh: ";
	if (ft_strnstr(type, "Numbers of args", ft_strlen(type)))
		ft_printfd(2, "%s%s: too many arguments\n", shell, name);
	else if (ft_strnstr(type, "Invalid flags", ft_strlen(type)))
	{
		if (error)
			ft_printfd(2, "%s%s: -%c: invalid option\n", shell, name, error);
		else
			ft_printfd(2, "%s%s: -: invalid option\n", shell, name);
		ft_printfd(2, "%s: usage: %s %s\n", name, name, flags);
		return (2);
	}
	else if (ft_strnstr(type, "HOME", ft_strlen(type)))
		ft_printfd(2, "%s%s: HOME not set\n", shell, name);
	else if (ft_strnstr(type, "System failed", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s\n", shell, name, strerror(errno));
	else if (ft_strnstr(type, "Not valid identifier", ft_strlen(type)))
		ft_printfd(2, "%s%s: `%s': not a valid identifier\n", shell, name,
			flags);
	else if (ft_strnstr(type, "Numeric arg required", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s: numeric argument required\n", shell, name,
			flags);
	return (1);
}

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

/**
 * COMENT PENDING
 */
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
