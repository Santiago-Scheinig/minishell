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
 * Initializes the enviromental variables needed to execute minishell.
 * 
 * @param minishell A pointer to the minishell enviroment structure.
 * @param envp The array of strings of the main envp.
 */
static void	init_envp(t_body *msh, const char **envp)
{
	char 	**ps1;

	msh->envp = shell_envpdup(envp);
	if (!msh->envp)
		forcend(msh, "malloc", MSHELL_FAILURE);
	msh->envp_lst = shell_newlst_var(msh->envp);
	shell_sortenv(&msh->envp_lst);
	if (!msh->envp_lst)
		forcend(msh, "malloc", MSHELL_FAILURE);
	ps1 = shell_pmtstr(msh->envp_lst);
	if (!ps1)
		forcend(msh, "malloc", MSHELL_FAILURE);
	shell_sortenv(&msh->envp_lst);
	if (!shell_getenv(msh->envp_lst, "PS1"))
	{
		msh_import(&msh->envp, &msh->envp_lst, ps1);
		shell_sortenv(&msh->envp_lst);
		ft_split_free(ps1);
	}
	else
		free(ps1);
}

/**
 * Initializes the setting of the terminal needed to execute minishell.
 * 
 * @param minishell A pointer to the minishell enviroment structure.
 * @note This allows msh to have two different modes, interactive and
 * automatic.
 */
static void	init_term(t_body *msh)
{
	struct termios	new_term;

	ft_memset(msh, 0, sizeof(t_body));
	msh->interactive = isatty(STDIN_FILENO);
	if (msh->interactive && isatty(STDOUT_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &(msh->orig_term)))
			forcend(msh, "tcgetattr", MSHELL_FAILURE);
		new_term = msh->orig_term;
		new_term.c_lflag |= ECHOCTL;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term))
			forcend(msh, "tcsetattr", MSHELL_FAILURE);
		if (sigquit() || sigint())
			forcend(msh, "sigaction", MSHELL_FAILURE);
	}
}

int	main(int argc, char **argv, const char **envp)
{
	t_body	msh;

	errno = 0;
	init_term(&msh);
	init_envp(&msh, envp);
	if (argc > 1 || argv[1])
	{
		errno = ENOENT;
		return (forcend(&msh, argv[1], MSHELL_FAILURE));
	}
	while (1)
	{
		if (parser(&msh))
			continue;
		if (execmd(&msh))
			continue;
		if (msh.childs_pid)
			waitcmd(&msh);
	}
	return (0);
}
