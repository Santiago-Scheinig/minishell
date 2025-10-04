/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 13:50:11 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 15:51:16 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

static int	print_value(char *value)
{
	if (value)
	{
		if (write(STDOUT_FILENO, "=\"", 2) == -1)
		return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, value, ft_strlen(value)) == -1)
		return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, "\"", 1) == -1)
		return (MSHELL_FAILURE);
	}
	return (MSHELL_SUCCESS);
}

/**
 * @param env_lst Linked list of t_var nodes containing name, value and
 * exported flag.
 * 
 * Iterates the list and writes each exported variable in the format:
 *   declare -x NAME="VALUE"
 * or just "declare -x NAME" if no value.
 */
int	print_export(t_list *env_lst)
{
	t_list	*current;
	t_var	*tmp;
	
	current = env_lst;
	while (current && current->content)
	{
		tmp = (t_var *)current->content;
		if (tmp->name && tmp->exported)
		{
			if (write(STDOUT_FILENO, "declare -x ", 11) == -1)
			return (MSHELL_FAILURE);
			if (write(STDOUT_FILENO, tmp->name, ft_strlen(tmp->name)) == -1)
			return (MSHELL_FAILURE);
			if (print_value(tmp->value))
			return (MSHELL_FAILURE);
			if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (MSHELL_FAILURE);
		}
		current = current->next;
	}
	return (MSHELL_SUCCESS);
}

/**
 * @param head Pointer to t_list that have the enviroments "NAME=VALUE".
 * 
 * try to print the export list and check if there was an error in a write
 * fuction
 * 
 * @return 0 on success, non-zero if a write fail.
 */
int	export(t_list **head)
{
	if (print_export(*head))
		return (built_end("export", "System failed", NULL, '\0'));
	return (MSHELL_SUCCESS);
}

/**
 * Prints the environment array (envp) to stdout, one per line.
 * 
 * @param envp Null-terminated array of environment strings ("NAME=VALUE").
 * 
 * Writes each env string followed by a newline.
 */
int	print_env(char **envp)
{
	int		i;

	i = 0;
	while (envp[i])
	{
		if (write(STDOUT_FILENO, envp[i], ft_strlen(envp[i])) == -1)
			return (MSHELL_FAILURE);
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (MSHELL_FAILURE);
		i++;
	}
	return (MSHELL_SUCCESS);
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
	if (print_env(envp))
		return (built_end(args[0], "System failed", NULL, '\0'));
	return (MSHELL_SUCCESS);
}
