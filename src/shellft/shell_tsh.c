/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tsh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 19:58:43 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 08:12:49 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

int	shell_hdocerr(char *limit, char *err_ft, int heredoc_fd[2], t_body *msh)
{
	const char	*msg[3] = {
		"msh: warning:",
		"here-document at line",
		"delimited by end-of-file",
	};

	if (errno == ENOMEM)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	else if (errno)
		msh->exit_ft = err_ft;
	else
	{
		ft_fprintf(2, "\n%s %s %i %s ", msg[0], msg[1], msh->line, msg[2]);
		ft_fprintf(2, "(wanted '%s')\n", limit);
		close(heredoc_fd[1]);
		return (heredoc_fd[0]);
	}
	free(limit);
	close(heredoc_fd[0]);
	close(heredoc_fd[2]);
	return (MSHELL_FAILURE);
}

/**
 * Porque no usamos error directamente para flags y error, al final si queres 
 * imprimir un solo caracter, podes mandar un string con un solo caracter.
 */
int	shell_binerr(int binerr, char *cmd, char *flags, char error)
{
	ft_fprintf(STDERR_FILENO, "%s: ", cmd);
	if (binerr == SYSFAIL)//this is a tricky fail
	{
		if (errno == ENOMEM)
			ft_fprintf(STDERR_FILENO, "malloc: %s\n", strerror(errno));
		else
			ft_fprintf(STDERR_FILENO, "write: %s\n", strerror(errno));
	}
	if (binerr == INVARGC)
		ft_fprintf(STDERR_FILENO, "too many arguments\n");
	if (binerr == INVARGV)
		ft_fprintf(STDERR_FILENO, "%s: numeric argument required\n", flags);//como que flags? si es un INVARG?
	if (binerr == INVFLGS)
	{
		if (error)
			ft_fprintf(STDERR_FILENO, "-%c: invalid option\n", error);//Esto es una flag con un character despues del -
		else
			ft_fprintf(STDERR_FILENO, "-: invalid option\n");//Osea, esto es una flag sin un character despues del -
		ft_printfd(STDERR_FILENO, "%s: usage: %s %s\n", cmd, cmd, flags);//Aca me pierdo de nuevo, INVFLGS tira un error por pantalla doble?
		return (MSHELL_MISSUSE);
	}
	if (binerr == INVIDFY)
		ft_fprintf(STDERR_FILENO, "'%s': not a valid identifier\n", flags);//Sigo sin intender que es flags, si no hay ninguna flag
	if (binerr == INVHOME)
		ft_fprintf(STDERR_FILENO, "HOME not set\n");
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
	shell_cleanup(true, msh);
	if (errno && argv)
	{
		ft_printfd(STDERR_FILENO, "msh: ");
		perror(argv);
	}
	if (exit_no != MSHELL_FATAL
	&& tcsetattr(STDIN_FILENO, TCSANOW, &msh->orig_term))
		exit(exit_no);
	exit(exit_no);
}
