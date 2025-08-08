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

#include "minishell.h"
#include "signals.h"
#include "parser.h"

/**
 * Short description of the function porpuse.
 * 
 * @param argc What this argument should be
 * @param argv What this argument should be
 * @param envp What this argument should be
 *  
 * Additional information about behaivor and how it works.
 * 
 * Verifies if [argc] is valid and works on [envp] to find cmd binaries.
 * 
 * To jump lines you must leave an empty one.
 * 
 * @return What does the function returns (If it returns something)
 * @note - A specific note about a particular behaivor or error.
 * @note - [argv] is not used
 * @note - You can add snippets using the "-" character at the beggining
 * 
 */

// temporal cleanup for tests
void	cleanup(t_body *minishell)
{
	if (minishell->input)
	{
		free(minishell->input);
		minishell->input = NULL;
	}
	if (minishell->token_lst)
	{
		ft_lstclear(&(minishell->token_lst), free);
		minishell->token_lst = NULL;
	}
	if (minishell->cmd_lst)
	{
		ft_lstclear(&(minishell->cmd_lst), free);
		minishell->cmd_lst = NULL;
	}
	if (minishell->childs_pid)
	{
		free(minishell->childs_pid);
		minishell->childs_pid = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_body	minishell;
	t_token	*test;

	if (argc != 1 || !argv[0])
		return (1);
	config_minishell(&minishell);
	memset(&minishell, 0, sizeof(minishell));
	minishell.envp = envp;
	while (1)
	{
		recive_signals(&minishell);
		parser(&minishell);
		if (!minishell.cmd_lst)
		{
			while (minishell.token_lst)
			{
				test = (t_token *) minishell.token_lst->content;
				printf("%s - %i\n", test->str, test->type);
				minishell.token_lst = minishell.token_lst->next;
			}
		}
		//execute(commands)
		//if only one cmd and it's built in - don't fork, any other way we fork.
		if (!ft_strncmp(minishell.input, "exit", 5))
		{
			cleanup(&minishell);
			break ;
		}
		else if (minishell.input[0] == '\0') //empty line
			continue ;
	}
	return (0);
}
