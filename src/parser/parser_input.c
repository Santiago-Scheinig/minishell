/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_input.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:52:57 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 07:38:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_bin.h"

/**
 * @brief	Temporarily redirects STDOUT to the terminal device.
 *
 *			Opens "/dev/tty" for read/write access and duplicates the
 *			current STDOUT file descriptor. Then replaces STDOUT with
 *			the terminal descriptor to allow prompt display even when
 *			output is redirected.
 *
 *			On failure, forces shell termination with an error message.
 *
 * @param	msh	Pointer to main shell structure (t_body).
 *
 * @note	Returned descriptor must be restored later with
 *			ttyout_to_stdout().
 *
 * @return	Copy of the original STDOUT file descriptor.
 */
static int	stdout_to_ttyout(t_body *msh)
{
	int	fd_tty;
	int	fd_outcpy;

	fd_tty = open("/dev/tty", O_RDWR);
	if (fd_tty == -1)
		shell_forcend(MSHELL_FAILURE, "readline", msh);
	fd_outcpy = dup(STDOUT_FILENO);
	if (fd_outcpy == -1)
	{
		close(fd_tty);
		shell_forcend(MSHELL_FAILURE, "readline", msh);
	}
	if (!dup2(fd_tty, STDOUT_FILENO))
	{
		close(fd_tty);
		shell_forcend(MSHELL_FAILURE, "readline", msh);
	}
	close(fd_tty);
	return (fd_outcpy);
}

/**
 * @brief	Restores STDOUT from a previously saved descriptor.
 *
 *			Replaces STDOUT with the provided backup descriptor to
 *			recover normal output redirection after a temporary
 *			switch to /dev/tty during prompt reading.
 *
 *			On failure, the shell exits with an error.
 *
 * @param	fd_outcpy	File descriptor copy of original STDOUT.
 * @param	msh			Pointer to main shell structure (t_body).
 *
 * @return	Always returns MSHELL_SUCCESS on success.
 */
static void	ttyout_to_stdout(int fd_outcpy, t_body *msh)
{
	if (dup2(fd_outcpy, STDOUT_FILENO))
	{
		close(fd_outcpy);
		shell_forcend(MSHELL_FAILURE, "readline", msh);
	}
	close(fd_outcpy);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Reads a command line from the user prompt.
 *
 *			If STDOUT is not a terminal, temporarily redirects it
 *			to /dev/tty so readline() can display properly.
 *
 *			Builds the shell prompt string using environment
 *			variables, reads user input with readline(), and returns
 *			the resulting line. On failure, the shell terminates.
 *
 *			Static helpers:
 *
 *				- stdout_to_ttyout():	Sets STDOUT to /dev/tty for
 *										readline compatibility.
 *
 *				- ttyout_to_stdout():	Restores original STDOUT.
 *
 * @param	msh	Pointer to main shell structure (t_body).
 *
 * @note	Returned string must be freed by the caller.
 *
 * @return	User input string or NULL on EOF (Ctrl+D).
 */
static char	*reader_prompt(t_body *msh)
{
	char	*input_usr;
	char	*tmp;
	int		tty;
	int		fd_outcpy;

	fd_outcpy = 0;
	tty = isatty(STDOUT_FILENO);
	if (!tty)
		fd_outcpy = stdout_to_ttyout(msh);
	tmp = shell_newpmt(msh->head_envar);
	if (!tmp)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	input_usr = readline(tmp);//ft_readline()
	free(tmp);
	if (!input_usr && errno)
		shell_forcend(MSHELL_FAILURE, "readline", msh);
	if (!tty && fd_outcpy)
		ttyout_to_stdout(fd_outcpy, msh);
	return (input_usr);
}

/**
 * @brief	Reads input from user or stdin depending on mode.
 *
 *			In interactive mode, prompts the user for input using
 *			reader_prompt(). Otherwise reads the next line from
 *			stdin using ft_get_next_line() and trims the newline.
 *
 *			On read or allocation failure, terminates the shell.
 *
 *			Static helpers:
 *
 *				- reader_prompt():	Handles prompt display and
 *									user input capture.
 *
 * @param	msh	Pointer to main shell structure (t_body).
 *
 * @note	Increments msh->line in non-interactive mode.
 *
 * @return	Input string. Exits shell on EOF or fatal error.
 */
static int	input_reader(t_body *msh)
{
	char	*input;
	char	*tmp;
	int		fd_outcpy;
	
	if (msh->interactive)
		input = reader_prompt(msh);
	else
	{
		tmp = ft_get_next_line(STDIN_FILENO);
		if (!tmp && errno == ENOMEM)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		else if (!tmp && errno)
			shell_forcend(MSHELL_FAILURE, "read", msh);
		input = ft_strtrim(tmp, "\n");
		free(tmp);
		if (!input && errno == ENOMEM)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		msh->line++;
	}
	if (!input)
		msh_exit(NULL, msh);
	return (input);
}

/**
 * @brief	Parses shell input into tokens for command execution.
 *
 *			Reads user input, splits it into tokens separated by
 *			spaces or special operators (<<, >>, <, >, |). Handles
 *			interactive history and empty input recursion.
 *
 *			Static helpers:
 *
 *				- input_reader():	Handles prompt or stdin reading.
 *
 * @param	split	Output pointer to array of tokenized strings.
 * @param	msh		Pointer to main shell structure (t_body).
 *
 * @note	Frees the input string after splitting.
 * @note	Recursively re-prompts on empty input.
 */
void	parser_input(char ***split, t_body *msh)
{
	char		*input;
	const char	*set[] = {
		"<<",
		">>",
		"<",
		">",
		"|",
		NULL,
	};

	input = input_reader(msh);
	if (!input[0])
	{
		free(input);
		parser_input(split, msh);
	}
	if (msh->interactive)
		add_history(input);
	(*split) = ft_split_iqbase(input, ' ', set);
	if (!(*split))
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	free(input);
}
