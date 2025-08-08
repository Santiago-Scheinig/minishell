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
	if (minishell->pipe_child)
	{
		free(minishell->pipe_child);
		minishell->pipe_child = NULL;
	}

}

static char	*pwd_name(char *user, char *path, t_body *minishell)
{
	char	*tmp1;
	char	*tmp2;
	
	tmp1 = ft_strjoin(user, ":");
	if (!tmp1)
	{
		free(path);
		free(minishell->prompt);
		return (NULL);
	}
	tmp2 = ft_strjoin(tmp1, path);
	free(path);
	if (!tmp1)
	{
		free(minishell->prompt);
		return (NULL);
	}
	minishell->prompt = ft_strjoin(tmp2, "$ ");
	if (!minishell->prompt)
		return (NULL);
	return (minishell->prompt);
}

static char	*path_cwd()
{
	char	*path;

	path = malloc(PATH_MAX * sizeof(char));
	if (!path)
	{
		perror("malloc");
		return (NULL);
	}
	if (!getcwd(path, PATH_MAX))
	{
		perror("pwd");
		free(path);
		return (NULL);
	}
	return (path);
}

static int	path_minishell(t_body *minishell)
{
	size_t	size;
	char	*user;
	char	*path;

	user = getenv("USER");
	if (!user)
	{
		perror("USER no define");
		return (0);
	}
	path = path_cwd();
	if (!path)
		return (0);
	size = ft_strlen(path) + 4 + ft_strlen(user);
	minishell->prompt = malloc(size * sizeof(char));
	if (!minishell->prompt)
	{
		free(path);
		return (0);
	}
	if (!pwd_name(user, path, minishell))
		return (0);
	return (1);
}

int	main(int argc, char **argv, char **envp)
{
	t_body	minishell;

	if (argc >= 2)
		return (1);
	initialization();
	if (argv || envp)
		ft_memset(&minishell, 0, sizeof(t_body));
	while (1)
	{
		if (!path_minishell(&minishell))
			return (1);
		recive_signals(&minishell);
		//commands = parser(input);
			//we tokenize and validate everithing, if works, returns a T_CMD **.
		//if (!commands)
			//continue; (input wasn't valid, parser() should print the error);
		//execute(commands)
		//if only one cmd and it's built in - don't fork, any other way we fork.
		if (!ft_strncmp(minishell.input, "exit", 5))
		{
			cleanup(&minishell);
			break ;
		}
		else if (minishell.input[0] == '\0') //empty line
			continue ;
		else
			ft_printf("Invalid command\n");
	}
	return (0);
}
