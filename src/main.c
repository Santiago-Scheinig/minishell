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
https://github.com/Santiago-Scheinig/minishell/pull/35/conflict?name=include%252Fminishell.h&ancestor_oid=0ca17f3df83afadea873b14aaf06c1326bdcb304&base_oid=f9b00f86bd4b4724c87fe68c4ae9f23e3abc27ba&head_oid=adfface19af4da9339a85db904208a67719edd83{
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
	if (minishell->childs_pid)
	{
		free(minishell->childs_pid);
		minishell->childs_pid = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_body	minishell;
	t_list	*aux;

	if (argc != 1 || !argv[0])
		return (1);
/* 	int i = -1;
	while(envp[++i])
		printf("%s\n", envp[i]); */
	config_minishell(&minishell);
	memset(&minishell, 0, sizeof(minishell));
	minishell.envp = envp;
	while (1)
	{
		recive_signals(&minishell); // We cut if from here
		parser(&minishell); // <-- An place it inside of parser as the first step "recive_user_input()"
		aux = minishell.cmd_lst;
		while (aux)
		{
			if (aux->content)
			{
				int i = -1;
				while (((t_cmd *) aux->content)->argv[++i])
					printf("argv[%i]: %s\n", i, ((t_cmd *) aux->content)->argv[i]);
				printf("infile: %i\n", ((t_cmd *) aux->content)->infile);
				printf("outfile: %i\n", ((t_cmd *) aux->content)->outfile);
				printf("limitator: %s\n", ((t_cmd *) aux->content)->limitator);
			}
			aux = aux->next;
		}
		//execute(commands)
		//if only one cmd and it's built in - don't fork, any other way we fork.
	}
	return (0);
}
