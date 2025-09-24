/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 20:15:05 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

/**
 * @param env_lst Linked list of t_var nodes containing name, value and
 * exported flag.
 * 
 * Iterates the list and writes each exported variable in the format:
 *   declare -x NAME="VALUE"
 * or just "declare -x NAME" if no value.
 */
void	print_export(t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;

	current = env_lst;
	while (current)
	{
		tmp = (t_var *)current->content;
		if (tmp->name && tmp->exported)
		{
			write(1, "declare -x ", 11);
			write(1, tmp->name, ft_strlen(tmp->name));
			if (tmp->value)
			{
				write(1, "=\"", 2);
				write(1, tmp->value, ft_strlen(tmp->value));
				write(1, "\"", 1);
			}
			write(1, "\n", 1);
		}
		current = current->next;
	}
}

/**
 * Prints the environment array (envp) to stdout, one per line.
 * 
 * @param envp Null-terminated array of environment strings ("NAME=VALUE").
 * 
 * Writes each env string followed by a newline.
 */
void	print_env(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		write(1, envp[i], ft_strlen(envp[i]));
		write(1, "\n", 1);
		i++;
	}
}

/**
 * Built-in 'env' command handler.
 * 
 * @param args Command arguments array (args[0] == "env").
 * @param envp Environment array to print.
 * 
 * Validates flags and argument count, then prints environment.
 * 
 * @return 0 on success, non-zero on error.
 */
int	msh_env(char **args, char **envp)
{
	int	i;

	i = 0;
	if (args[1] && args[1][0] == '-')
		return (built_end(args[0], "Invalid flags", "[]", args[1][1]));
	else if (args[1])
		return (built_end(args[0], "Numbers of args", NULL, '\0'));
	print_env(envp);
	return (0);
}
