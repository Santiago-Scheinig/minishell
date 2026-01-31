/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildc_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/12 17:39:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

/**
 * @brief	Reads the current directory and collects visible file names.
 *
 *			Opens the current directory (".") and iterates over all
 *			entries. Hidden files (starting with '.') are skipped.
 *			Each name is copied into the provided names array via
 *			copy_name(). On allocation failure, exits via shell_forcend().
 *
 * @param	names	Pointer to an array of strings to store file names.
 * @param	msh		Pointer to the shell state for memory/error handling.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on opendir failure.
 */
static int	dir_names(char ***names, t_body *msh)
{
	struct dirent	*entry;
	DIR				*dir;

	dir = opendir(".");
	if (!dir)
	{
		perror("opendir");
		return (MSHELL_FAILURE);
	}
	while (1)
	{
		entry = readdir(dir);
		if (entry == NULL)
			break ;
		if (entry->d_name[0] == '.')
			continue ;
		if (copy_name(names, entry->d_name))
		{
			closedir(dir);
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		}
	}
	closedir(dir);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Adds a list of strings as new tokens to a token list.
 *
 *			Iterates through split[] in reverse order and adds each
 *			element to token_lst as a new token. Frees the split array
 *			after insertion. On memory allocation failure, calls
 *			shell_forcend().
 *
 * @param	split		Array of strings to add as tokens.
 * @param	token_lst	Linked list of tokens to append to.
 * @param	msh			Shell state pointer for memory/error handling.
 */
static void	add_to_list(char **split, t_list *token_lst, t_body *msh)
{
	int		i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
		i++;
	while (i && split[--i])
	{
		if (!split[1])
		{
			ft_split_free(split);
			split = NULL;
			break ;
		}
		if (shell_lstadd_newtkn(i, split[i], OPERATORS, token_lst))
		{
			ft_split_free(split);
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		}
	}
	free(split);
}

/**
 * @brief	Expands wildcard patterns in tokens to matching filenames.
 *
 *			Builds a list of names in the current directory, then for
 *			each token in msh->head_token, finds matches via wildcard().
 *			Matching strings are added as new tokens. Frees temporary
 *			memory arrays. On allocation failure, calls shell_forcend().
 *
 * @param	msh		Shell state containing token list and environment.
 *
 * @return	MSHELL_SUCCESS on success.
 */
int	parser_wildcard(t_body *msh)
{
	t_list	*token_lst;
	char	**matches;
	char	**names;

	token_lst = msh->head_token;
	names = NULL;
	dir_names(&names, msh);
	while (token_lst)
	{
		matches = NULL;
		if (wildcard(names, &matches, token_lst))
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		add_to_list(matches, token_lst, msh);
		token_lst = token_lst->next;
	}
	ft_split_free(names);
	return (MSHELL_SUCCESS);
}
