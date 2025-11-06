/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:10:04 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 13:14:26 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include "msh_psr.h"
#include "msh_psr_bonus.h"
#include "msh_exe.h"

/**
 * @brief	Main parsing routine for minishell input processing.
 *
 *			Coordinates all parsing stages required to transform
 *			user input into executable command structures. Handles
 *			signal interruptions, lexical splitting, tokenization,
 *			environment variable expansion, and command setup.
 *
 *			Parsing sequence:
 *
 *				1. shell_sigint_read() — handles SIGINT on input.
 *				2. shell_cleanup() — resets parser state.
 *				3. parser_input() — retrieves and splits input line.
 *				4. parser_token() — builds token list.
 *				5. parser_envar() — expands environment variables.
 *				6. parser_cmds() — builds executable command list.
 *
 * @param	msh	Main shell structure holding runtime state,
 *			environment, tokens, and command lists.
 *
 * @note	On parsing or memory errors, exits via shell_forcend().
 * @note	Frees token list before returning successfully.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_SIG_HANDLR if a
 *			signal was caught during input.
 */
static int	parser(char *input, t_body *msh)
{
	char	**split;

	shell_cleanup(false, msh);
	if (parser_input(input, &split, msh))
		return (MSHELL_FAILURE);
	if (parser_token(split, msh))
	{
		if (!msh->interactive && msh->exit_no == MSHELL_MISSUSE)
			shell_forcend(msh->exit_no, NULL, msh);
		if (msh->exit_no == MSHELL_FAILURE)
			shell_forcend(msh->exit_no, msh->exit_ft, msh);
		return (msh->exit_no);
	}
	parser_envar(msh);
	parser_wildcard(msh);
	parser_cmds(msh);
	ft_lstclear(&(msh->head_token), shell_deltkn);
	msh->head_token = NULL;
	if (execution(msh))
		return (MSHELL_FAILURE);
	if (msh->childs_pid)
		waitexec(msh);
	if (msh->input_result == MSHELL_FAILURE)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

int	logic_subshell(char	*input, t_body *msh)
{
	char	*smallest_input;
	int		subshell_pid;
	int		status;

	subshell_pid = fork();
	if (!subshell_pid)
	{
		input = ft_strchr(input, '(') + 1;
		smallest_input = ft_calloc(ft_strlen_chr(input, ')') + 1, sizeof(char));
		if (!smallest_input)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		ft_strlcpy(smallest_input, input, ft_strlen_chr(input, ')'));
		exit (logic_execution(smallest_input, msh));
	}
	if (waitpid(subshell_pid, &status, 0) == -1)
		perror("msh: waitpid");
	msh->exit_no = check_status(status, STDERR_FILENO, 0, 0);
	if (msh->exit_no)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);	
}

int	logic_execution(char *input, t_body *msh)
{
	char		*smallest_input;
	const char	*operator = NULL;

	if (ft_strnstr_ip(input, "||", ft_strlen(input)))
		operator = "||";
	else if (ft_strnstr_ip(input, "&&", ft_strlen(input)))
		operator = "&&";
	if (operator)
	{
		smallest_input = ft_calloc(ft_strlen_chr(input, operator) + 1, sizeof(char));
		if (!smallest_input)
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		ft_strlcpy(smallest_input, input, ft_strlen_chr(input, operator) + 1);
		logic_execution(smallest_input, msh);
		if (operator == "||" && msh->input_result)
			logic_execution(ft_strnstr_ip(input, operator, ft_strlen(input)) + 2, msh);
		else if (operator == "&&" && !msh->input_result)
			logic_execution(ft_strnstr_ip(input, operator, ft_strlen(input)) + 2, msh);
		return (MSHELL_SUCCESS);
	}
	else if (ft_strchr(input, '('))
		return (logic_subshell(input, msh));
	if (shell_sigint_read(msh))
		return (MSHELL_SIG_HANDLR);
	return (parser(input, msh));
}

/**
 * @brief	Initializes the main minishell state and terminal environment.
 *
 *			Sets up the t_body structure, configures terminal attributes
 *			for interactive sessions, applies signal handlers, and loads
 *			the environment variables into both array and list formats.
 *
 * @param	msh	Pointer to the main shell structure to initialize.
 *
 * @note	If the shell runs interactively, echo control characters
 *			(like ^C) are displayed using ECHOCTL mode.
 * @note	On any initialization error (termios or malloc failure),
 *			the shell terminates immediately through shell_forcend().
 *
 * @return	None. Exits the program if any step fails.
 */
static void	msh_init(const char **envp, t_body *msh)
{
	struct termios	new_term;

	ft_memset(msh, 0, sizeof(t_body));
	msh->interactive = isatty(STDIN_FILENO);
	if (msh->interactive)
	{
		if (tcgetattr(STDIN_FILENO, &(msh->orig_term)))
			shell_forcend(MSHELL_FATAL, "tcgetattr", msh);
		new_term = msh->orig_term;
		new_term.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term))
			shell_forcend(MSHELL_FATAL, "tcsetattr", msh);
		if (shell_sigquit() || shell_sigint(msh->interactive))
			shell_forcend(MSHELL_FAILURE, "sigaction", msh);
	}
	else if (shell_sigint(msh->interactive))
		shell_forcend(MSHELL_FAILURE, "sigaction", msh);
	if (shell_envini((char **) envp, &(msh->envp), &(msh->head_envar)))
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
}

/**
 * @brief Entry point for the mini shell program.
 *
 * Initializes the shell environment, including terminal settings and
 * environment variables. Enters the main shell loop, handling parsing,
 * command execution, and waiting for child processes.
 *
 * @param argc	Number of command-line arguments.
 * @param argv	Array of command-line argument strings.
 * @param envp	Array of environment variable strings.
 *
 * @note	If command-line arguments are provided, the shell returns an error
 *			(ENOENT) and exits immediately.
 * @note	The main loop normally runs indefinitely in interactive mode until
 *			the shell is externally terminated. In non-interactive mode, the
 *			loop continues until input ends or a syntax error occurs.
 * @note	errno is reset to 0 before parsing, executing, and waiting for
 *			commands to ensure accurate error handling.
 *
 * @return	The exit status stored in 'msh.exit_no', which reflects the last
 *			executed command's exit code, or MSHELL_FAILURE if an invalid
 *			argument is provided at startup.
 */
int	main(int argc, char **argv, const char **envp)
{
	t_body	msh;
	char	*logic_input;

	errno = ENOENT;
	msh_init(envp, &msh);
	if (argc > 1 || argv[1])
		shell_forcend(MSHELL_FAILURE, argv[1], &msh);
	while (1)
	{
		logic_input = NULL;
		if (logic_parser(&logic_input, &msh))
			continue;
		if (shell_sigint_read(&msh))
			continue;
		logic_execution(logic_input, &msh);
		free(logic_input);
	}
	return (msh.exit_no);
}
