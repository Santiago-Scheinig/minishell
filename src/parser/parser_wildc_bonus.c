/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/12 14:26:51 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include <dirent.h> //bonus

static int	dir_names(char ***names, t_body *msh)
{
	struct dirent	*entry;
	DIR				*dir;

	dir = opendir(".");
	if (!dir)
		forcend(msh, "opendir", MSHELL_FAILURE);
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
			forcend(msh, "malloc", MSHELL_FAILURE);
		}
	}
	closedir(dir);
	return (MSHELL_SUCCESS);
}

static void	add_to_list(char **split, t_list *token_lst, t_body *msh)
{
	int		i;

	i = 0;
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
		if (shell_addlst_token(token_lst, split[i], i))
		{
			ft_split_free(split);
			forcend(msh, "malloc", MSHELL_FAILURE);
		}
	}
	if (split)
		free(split);
}

int	parser_wildcard(t_body *msh)
{
	t_list	*token_lst;
	char	**matches;
	char	**names;
	
	token_lst = msh->token_lst;
	names = NULL;
	if (dir_names(&names, msh))
		return (MSHELL_FAILURE);
	while (token_lst)
	{
		matches = NULL;
		if (wildcard(names, &matches, token_lst))
			forcend(msh, "malloc", MSHELL_FAILURE);
		add_to_list(matches, token_lst, msh);
		token_lst = token_lst->next;
		free(matches);
	}
	ft_split_free(names);
	return (MSHELL_SUCCESS);
}
