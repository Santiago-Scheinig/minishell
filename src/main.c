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
int main(int argc, char **argv, char **envp)
{
	char	*input;
	//t_cmd	**commands;

	if (argc >= 2)
		return (1);
	if (argv || envp)
		input = NULL;
	while (1)
	{
		input = readline("minishell> ");
		//if (input[0])
			//add to history (only non-empty lines);
		//if (!input)
			//is it an error?
		//commands = parser(input);
			//we tokenize and validate everithing, if works, returns a T_CMD **.
		//if (!commands)
			//continue; (input wasn't valid, parser() should print the error);
		//execute(commands)
			//if only one cmd and it's built in - don't fork, any other way we fork.
		if (!ft_strncmp(input, "exit", 5))
		{
			free(input);
			break ;
		}
		else
		{
			free(input);
			ft_printf("Invalid command\n");
		}
	}
	return (0);
}
