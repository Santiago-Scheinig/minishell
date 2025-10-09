/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tsh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 05:57:55 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Cleans up all resources associated with the shell session.
 *
 *			Frees memory for the environment array, error file descriptor,
 *			child process IDs, token list, command list, and environment
 *			variable list. Clears the readline history if in interactive mode.
 *			Finally, zeroes the t_body structure using ft_memset.
 *
 * @param	msh	Pointer to the main shell structure (t_body) to clean.
 *
 * @note	Use this function before exiting the shell to prevent memory leaks.
 * @note	After calling, the t_body structure is reset to zero.
 */
void	shell_cleanup(t_body *msh)
{
	if (msh->interactive)
		rl_clear_history();
	if (msh->envp)
		ft_split_free(msh->envp);
	if (msh->err_fd)
		free(msh->err_fd);
	if (msh->childs_pid)
		free(msh->childs_pid);
	if (msh->head_token)
		ft_lstclear(&(msh->head_token), shell_deltkn);
	if (msh->head_cmd)
		ft_lstclear(&(msh->head_cmd), shell_delcmd);
	if (msh->head_envar)
		ft_lstclear(&msh->head_envar, shell_delenvar);
	ft_memset(msh, 0, sizeof(t_body));
}

/**
 * missing modifications
 */
int	shell_builterr(char *name, char *type, char *flags, char error)
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
	return (MSHELL_FAILURE);
}

/**
 * @brief	Reports an error related to redirection or pipe operations.
 *
 *			Prints a formatted error message to STDERR using perror(). If
 *			argv is NULL, defaults to "pipe". Returns the provided exit code.
 *
 * @param	exit_no	The exit code to return after reporting the error.
 * @param	argv	Pointer to the string describing the failing operation,
 *			or NULL to default to "pipe".
 *
 * @note	Does not terminate the shell; caller handles continuation or exit.
 *
 * @return	The provided exit code (exit_no).
 */
int	shell_redirerr(int exit_no, char *argv)
{
	if (!argv)
		argv = "pipe";
	ft_printfd(STDERR_FILENO, "msh: ");
	perror(argv);
	return (exit_no);
}

/**
 * @brief	Reports a syntax error for an unexpected token in the shell.
 *
 *			Prints a formatted error message to STDERR indicating the
 *			unexpected token. Includes the line number if the shell is
 *			not in interactive mode. Sets msh->exit_no to MSHELL_MISSUSE.
 *
 * @param	next	Pointer to the unexpected token string, or "newline" if NULL.
 * @param	msh		Pointer to the main shell structure (t_body) to update exit_no.
 *
 * @note	Returns the error code MSHELL_MISSUSE.
 * @note	Does not terminate the shell; execution continues after reporting.
 *
 * @return	MSHELL_MISSUSE.
 */
int	shell_parserr(const char *next, t_body *msh)
{
	const char	*msg = "syntax error near unexpected token";

	if (!next)
		next = "newline";
	ft_printfd(STDERR_FILENO, "msh: ");
	if (!msh->interactive)
		ft_printfd(STDERR_FILENO, "line: %i: ", msh->line);
	ft_printfd(STDERR_FILENO, "%s \'%s\'\n", msg, next);
	msh->exit_no = MSHELL_MISSUSE;
	return (MSHELL_MISSUSE);
}

/**
 * @brief	Forcefully exits the shell, optionally printing an error.
 *
 *			Calls shell_cleanup() to free all resources. If errno is set,
 *			prints a formatted error message including the argv context.
 *			Restores terminal attributes if necessary before exiting.
 *
 * @param	exit_no The exit code to terminate the shell with.
 * @param	argv 	Context string, usually the command that caused the error.
 * @param	msh		Pointer to the main shell structure (t_body) for cleanup.
 *
 * @note	Exits the program immediately; does not return.
 * @note	Use MSHELL_FATAL for unrecoverable errors.
 */
void	shell_forcend(int exit_no, const char *argv, t_body *msh)
{
	cleanup(msh);
	if (errno)
	{
		ft_printfd(STDERR_FILENO, "msh: ");
		perror(argv);
		if (exit_no != MSHELL_FATAL
			&& tcsetattr(STDIN_FILENO, TCSANOW, &msh->orig_term))
			exit(exit_no);
	}
	exit(exit_no);
}
