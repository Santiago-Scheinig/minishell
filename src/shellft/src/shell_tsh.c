/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tsh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:45:00 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Handles errors related to here-documents.
 *
 * 			Checks errno and prints appropriate warnings for
 *			here-document issues. Handles memory allocation errors,
 *			end-of-file without delimiter, and normal closure of
 *			the here-document pipe.
 *
 * @param	limit	String used as the delimiter for the here-document.
 * @param	errft	Pointer to store the error function context if needed.
 * @param	hdoc_fd	Array containing here-document pipe file descriptors.
 * @param	msh		Pointer to the shell context structure.
 *
 * @return	On success returns the reading end of the here-doc pipe,
 *			else returns MSHELL_FAILURE.
 */
int	shell_hderr(char *limit, char *errft, int hdoc_fd[2], t_body *msh)
{
	const char	*msg[3] = {
		"msh: warning:",
		"here-document at line",
		"delimited by end-of-file",
	};

	if (errno == ENOMEM)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	else if (errno)
		msh->exit_ft = errft;
	else
	{
		ft_fprintf(2, "\n%s %s %i %s ", msg[0], msg[1], msh->line, msg[2]);
		ft_fprintf(2, "(wanted '%s')\n", limit);
		close(hdoc_fd[1]);
		return (hdoc_fd[0]);
	}
	free(limit);
	close(hdoc_fd[0]);
	close(hdoc_fd[2]);
	return (MSHELL_FAILURE);
}

/**
 * @brief	Prints standardized error messages for built-in commands.
 *
 * 			Handles multiple types of errors: invalid flags, numeric
 *			argument errors, too many arguments, invalid identifiers,
 *			and system call failures. Prints usage messages as needed.
 *
 * @param	binerr	Integer code representing the type of error.
 * @param	cmd		Command name associated with the error.
 * @param	usage	String describing expected usage or argument.
 * @param	flag	Character representing the invalid flag, if any.
 *
 * @return	MSHELL_FAILURE in general, or MSHELL_MISSUSE if a flag
 *			error is present.
 */
int	shell_binerr(int binerr, char *cmd, char *usage, char flag)
{
	if (binerr == INVFLGS && flag)
		ft_fprintf(STDERR_FILENO, "-%c: invalid option\n", flag);
	else if (binerr == INVFLGS)
		ft_fprintf(STDERR_FILENO, "-: invalid option\n");
	ft_fprintf(STDERR_FILENO, "%s: ", cmd);
	if (binerr == SYSFAIL)
		perror(usage);
	if (binerr == INVARGC)
		ft_fprintf(STDERR_FILENO, "too many arguments\n");
	if (binerr == INVARGV)
		ft_fprintf(STDERR_FILENO, "[%s]: numeric argument required\n", usage);
	if (binerr == INVFLGS)
		ft_fprintf(STDERR_FILENO, "usage: %s [%s]\n", cmd, usage);
	if (binerr == INVIDFY)
		ft_fprintf(STDERR_FILENO, "'%s': not a valid identifier\n", usage);
	if (binerr == INVHOME)
		ft_fprintf(STDERR_FILENO, "HOME not set\n");
	if (flag)
		return (MSHELL_MISSUSE);
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
	ft_fprintf(STDERR_FILENO, "msh: ");
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
 * @param	msh		Pointer to the main shell structure (t_body) to update
 * 					exit_no.
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
	ft_fprintf(STDERR_FILENO, "msh: ");
	if (!msh->interactive)
		ft_fprintf(STDERR_FILENO, "line: %i: ", msh->line);
	ft_fprintf(STDERR_FILENO, "%s \'%s\'\n", msg, next);
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
	shell_cleanup(true, msh);
	if (errno && argv)
	{
		ft_fprintf(STDERR_FILENO, "msh: ");
		perror(argv);
	}
	if (exit_no != MSHELL_FATAL
		&& tcsetattr(STDIN_FILENO, TCSANOW, &msh->orig_term))
		exit(exit_no);
	exit(exit_no);
}
