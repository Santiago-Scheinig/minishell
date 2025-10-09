/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_tsh_del.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:32:21 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 03:02:32 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell_std.h"

/**
 * @brief	Frees a t_var structure and its allocated name and value strings.
 *
 *			Frees the name and value fields if they exist, then frees the
 *			t_var structure itself.
 *
 * @param	content	Pointer to the t_var structure to delete.
 *
 * @note	Use this function as a cleanup callback for linked lists of environment variables.
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
		free(word->str);
	if (tkn->mask)
		free(word->mask);
	tkn->str = NULL;
	tkn->mask = NULL;
	free(tkn);
}
