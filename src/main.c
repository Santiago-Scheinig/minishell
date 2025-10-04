/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:50:09 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/24 15:50:09 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh.h"
#include <sys/stat.h>
#include <unistd.h>

/**
 * @brief Initializes the shell environment variables.
 *
 * Duplicates the provided environment array, converts it into a linked
 * list of t_var, sorts it, and ensures that the PS1 prompt variable
 * exists. Allocates memory for the internal shell structures.
 *
 * @param envp	Pointer to the array of environment variable strings.
 * @param msh	Pointer to the shell state structure (t_body) to update.
 *
 * @note	Exits the shell using forcend on any memory allocation failure.
 * @note	Sets msh->envp (duplicated array) and msh->lst_t_var (linked list).
 * @note	If PS1 is not defined in envp, a default prompt is created and
 *			added to the environment.
 * @note	Memory allocated for PS1 strings is freed appropriately.
 * @note	Helper functions:
 *			 - shell_ functions return NULL on failure (except sortenv).
 *			 - msh_ functions return 1 on failure, 2 on misusage, 0 on success.
 *
 * @return	Nothing (void function). Exits on fatal error.
 */
static void	init_envp(const char **envp, t_body *msh)
{
	char	**ps1;

	msh->envp = shell_envpdup(envp);
	if (!msh->envp)
		forcend(msh, "malloc", MSHELL_FAILURE);
	msh->lst_t_var = shell_newlst_var(msh->envp);
	if (!msh->lst_t_var && errno)
		forcend(msh, "malloc", MSHELL_FAILURE);
	shell_sortenv(&(msh->lst_t_var));
	ps1 = shell_pmtstr(msh->lst_t_var);
	if (!ps1)
		forcend(msh, "malloc", MSHELL_FAILURE);
	shell_sortenv(&(msh->lst_t_var));
	if (!shell_getenv(msh->lst_t_var, "PS1"))
	{
		if (msh_export(&msh->envp, &msh->lst_t_var, &ps1[1]))
			forcend(msh, "malloc", MSHELL_FAILURE);
		if (msh_import(&msh->envp, &msh->lst_t_var, ps1))
			forcend(msh, "malloc", MSHELL_FAILURE);
		shell_sortenv(&msh->lst_t_var);
		ft_split_free(ps1);
	}
	else
		free(ps1);
}

/**
 * @brief Initializes the shell's terminal settings and signal handlers.
 *
 * Resets the provided t_body structure, checks whether the shell is running
 * in interactive mode, and configures terminal attributes accordingly.
 * In interactive mode, it enables the ECHOCTL flag and sets up signal
 * handlers for SIGINT and SIGQUIT. In non-interactive mode, it sets up
 * a SIGINT handler for abrupt exit.
 *
 * @param msh	Pointer to the shell state structure (t_body) to initialize.
 *
 * @note	Uses fstat and isatty to determine if the shell is interactive.
 * @note	In interactive mode, stores the original terminal settings in
 *			msh->orig_term.
 * @note	Calls forcend on error conditions (tcgetattr, tcsetattr, or
 *			signal setup) and exits the shell.
 * @return	Nothing (void function). Exits the program on critical errors.
 */
static void	init_term(t_body *msh)
{
	struct termios	new_term;
	struct stat		st;

	ft_memset(msh, 0, sizeof(t_body));
	fstat(STDIN_FILENO, &st);
	msh->interactive = isatty(STDIN_FILENO);
	if (msh->interactive)
	{
		if (tcgetattr(STDIN_FILENO, &(msh->orig_term)))
			forcend(msh, "tcgetattr", MSHELL_FAILURE);
		new_term = msh->orig_term;
		new_term.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term))
			forcend(msh, "tcsetattr", MSHELL_FAILURE);
		if (shell_sigquit() || shell_sigint())
			forcend(msh, "sigaction", MSHELL_FAILURE);
	}
	else if (shell_signint())
		forcend(msh, "sigaction", MSHELL_FAILURE);
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

	errno = 0;
	init_term(&msh);
	init_envp(envp, &msh);
	if (argc > 1 || argv[1])
	{
		errno = ENOENT;
		return (forcend(&msh, argv[1], MSHELL_FAILURE));
	}
	while (1)
	{
		errno = 0;
		if (parser(&msh))
			continue ;
		errno = 0;
		if (execmd(&msh))
			continue ;
		errno = 0;
		if (msh.childs_pid)
			waitcmd(&msh);
	}
	return (msh.exit_no);
}
