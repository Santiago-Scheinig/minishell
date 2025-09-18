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

/**
 * Global flag use to track signals.
 * 
 * This flag is used to notify the shell that the signal was intercepted,
 * allowing proper prompt refresh and cleanup.
 * 
 * @note Declared volatile because it is modified from within a signal handler.
 */

// Creates an array of signals so i can then set it with 
// sigmask to ign on childs or non-interactive mode
// static int	sigset(void)

static void	init_envp(t_body *minishell, const char **envp)
{
	char	**ps1;

	minishell->envp = shell_envpdup(envp);
	if (!minishell->envp)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	minishell->envp_lst = shell_newlst_var(minishell->envp);
	shell_sortenv(&minishell->envp_lst);
	if (!minishell->envp_lst)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	ps1 = shell_pmtstr(minishell->envp_lst);
	if (!ps1)
		forcend(minishell, "malloc", MSHELL_FAILURE);
	msh_import(&minishell->envp, &minishell->envp_lst, ps1);
	ft_split_free(ps1);
	shell_sortenv(&minishell->envp_lst);
}

static void	init_term(t_body *minishell)
{
	struct termios	new_term;

	ft_memset(minishell, 0, sizeof(t_body));
	minishell->interactive = isatty(STDIN_FILENO);
	if (minishell->interactive && isatty(STDOUT_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &(minishell->orig_term)))
			forcend(minishell, "tcgetattr", MSHELL_FATAL);
		new_term = minishell->orig_term;
		new_term.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term))
			forcend(minishell, "tcsetattr", MSHELL_FATAL);
		if (sigint() || sigquit())
			forcend(minishell, "sigaction", MSHELL_FAILURE);
	}
	else if (sigint() || sigquit())
			forcend(minishell, "signal", MSHELL_FAILURE);
}

int	main(int argc, char **argv, const char **envp)
{
	t_body	minishell;

	init_term(&minishell);
	init_envp(&minishell, envp);
	if (argc > 1 || argv[1])
		return (forcend(&minishell, argv[1], MSHELL_CMD_NOTEXE));
	while (1)
	{
		if (parser(&minishell))
			continue;
		// if (execmd(&minishell))
		// 	continue;
		//waitcmd(&minishell);
	}
	return (0);
}
