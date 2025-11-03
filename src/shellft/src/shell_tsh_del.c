/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tsh_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:32:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:47:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Cleans up allocated resources of the shell.
 *
 * 			Frees memory and clears structures related to environment
 * 			variables, child processes, command lists, token lists, and
 * 			error file descriptors. Optionally clears readline history if
 * 			the shell is in interactive mode and end flag is set.
 *
 * @param	end		Integer flag indicating if shell is terminating. If
 *					true, additional cleanup is performed for interactive
 *					mode and environment variables.
 * @param	msh		Pointer to the shell state structure containing all
 *					allocated resources and metadata.
 *
 * @note	After cleanup, all pointers in the msh structure are set to NULL.
 * @note	Do not use msh pointers after this function without reinitializing.
 */
void	shell_cleanup(int end, t_body *msh)
{
	if (end && msh->interactive)
		rl_clear_history();
	if (end && msh->envp)
		ft_split_free(msh->envp);
	if (msh->err_fd)
		free(msh->err_fd);
	if (msh->childs_pid)
		free(msh->childs_pid);
	if (msh->head_token)
		ft_lstclear(&(msh->head_token), shell_deltkn);
	if (msh->head_cmd)
		ft_lstclear(&(msh->head_cmd), shell_delcmd);
	msh->err_fd = NULL;
	msh->childs_pid = NULL;
	msh->head_token = NULL;
	msh->head_cmd = NULL;
	if (end && msh->head_envar)
		ft_lstclear(&msh->head_envar, shell_delenvar);
	if (end)
	{
		msh->head_envar = NULL;
		msh->envp = NULL;
	}
}

/**
 * @brief	Frees a t_var structure and its allocated name and value strings.
 *
 *			Frees the name and value fields if they exist, then frees the
 *			t_var structure itself.
 *
 * @param	content	Pointer to the t_var structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists of
 * 			environment variables.
 * @note	After calling, the pointer to the variable becomes invalid.
 */
void	shell_delenvar(void *content)
{
	t_var	*envar;

	envar = (t_var *) content;
	if (envar->name)
		free(envar->name);
	if (envar->value)
		free(envar->value);
	free(envar);
}

/**
 * @brief	Frees a t_cmd structure and all its associated resources.
 *
 *			Frees each string in the argv array and then the array itself.
 *			Frees the pathname string if it exists. Sets pointers to NULL
 *			before freeing the t_cmd structure.
 *
 * @param	content	Pointer to the t_cmd structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists of commands.
 * @note	After calling, the pointer to the command becomes invalid.
 */
void	shell_delcmd(void *content)
{
	t_cmd	*cmd;
	int		i;

	cmd = (t_cmd *) content;
	if (cmd->argv)
	{
		i = -1;
		while (cmd->argv[++i])
			free(cmd->argv[i]);
		free(cmd->argv);
	}
	if (cmd->pathname)
		free(cmd->pathname);
	cmd->argv = NULL;
	cmd->pathname = NULL;
	free(cmd);
}

/**
 * @brief	Frees a t_token structure and its allocated members.
 *
 *			Frees the str and mask fields of the token if they exist,
 *			sets them to NULL, and then frees the t_token structure itself.
 *
 * @param	content	Pointer to the t_token structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists of tokens.
 * @note	After calling, the pointer to the token becomes invalid.
 */
void	shell_deltkn(void *content)
{
	t_token	*tkn;

	tkn = (t_token *) content;
	if (tkn->str)
		free(tkn->str);
	if (tkn->mask)
		free(tkn->mask);
	tkn->str = NULL;
	tkn->mask = NULL;
	free(tkn);
}
