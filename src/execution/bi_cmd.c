/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bi_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 17:52:58 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/18 19:50:48 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_cmd.h"

/**
 * Handles error messages for built-in shell commands and print stderr.
 * Depending on the error type and flags, it formats and outputs the
 * appropriate message.
 * 
 * @param name The name of the command that caused the error.
 * @param type A string indicating the type of error to handle.
 * @param flags Additional flags or arguments related to the error.
 * @param error A character representing a specific invalid flag option
 * @return Returns 2 for invalid flag errors, otherwise returns 1.
 * 
 * @note Prints error messages prefixed with "minishell:".
 * @note Supports various error types such as argument count, Invalid flags,
 * missing HOME, System errors, Invalid identifiers, and Numeric argument
 * errors.
 */
int	built_end(char *name, char *type, char *flags, char error)
{
	char	*shell;

	shell = "minishell: ";
	if (ft_strnstr(type, "Numbers of args", ft_strlen(type)))
		ft_printfd(2, "%s%s: too many arguments\n", shell, name);
	else if (ft_strnstr(type, "Invalid flags", ft_strlen(type)))
	{
		if (error)
			ft_printfd(2, "%s%s: -%c: invalid option\n", shell, name, error);
		else
			ft_printfd(2, "%s%s: -: invalid option\n", shell, name);
		ft_printfd(2, "%s: usage: %s %s\n", name, name, flags);
		return (2);
	}
	else if (ft_strnstr(type, "HOME", ft_strlen(type)))
		ft_printfd(2, "%s%s: HOME not set\n", shell, name);
	else if (ft_strnstr(type, "System failed", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s\n", shell, name, strerror(errno));
	else if (ft_strnstr(type, "Not valid identifier", ft_strlen(type)))
		ft_printfd(2, "%s%s: `%s': not a valid identifier\n", shell, name,
			flags);
	else if (ft_strnstr(type, "Numeric arg required", ft_strlen(type)))
		ft_printfd(2, "%s%s: %s: numeric argument required\n", shell, name,
			flags);
	return (1);
}

/**
 * Executes the appropriate built-in shell command based on the
 * command name.
 * Calls the corresponding function for commands like export, cd, env, pwd,
 * echo, exit, unset, and unexport(envp not exported).
 *
 * @param minishell Pointer to the main shell structure containing environment
 * variables.
 * @param pathname The name of the built-in command to execute.
 * @param args The arguments passed to the command.
 * @param lst A linked list node containing environment variable data.
 * @return Returns the pathname of the executed built-in command.
 *
 * @note This function assumes commands are matched by name and delegates
 * execution accordingly.
 */
int built_in(char **args, t_body *minishell)
{
	if (!ft_strncmp(args[0], "export", 7))
		return (msh_export(&minishell->envp, &minishell->envp_lst, &args[1]));
	else if (!ft_strncmp(args[0], "cd", 3))
		return (msh_cd(args, minishell->envp_lst));
	else if (!ft_strncmp(args[0], "env", 4))
		return (msh_env(args, &minishell->envp[0], minishell->envp_lst));
	else if (!ft_strncmp(args[0], "pwd", 4))
		return (msh_pwd(args));
	else if (!ft_strncmp(args[0], "echo", 5))
		msh_echo(args);
	else if (!ft_strncmp(args[0], "unset", 6))
		return (msh_unset(minishell->envp, minishell->envp_lst, &args[1]));
	else if (!ft_strncmp(args[0], "exit", 5))
	{
		msh_exit(args, minishell);
		return (1);
	}
	else
		return (msh_import(&minishell->envp, &minishell->envp_lst, args));
	return (0);
}

/*int	exe_built(t_cmd *exe, t_body *minishell)
{
	
}*/
