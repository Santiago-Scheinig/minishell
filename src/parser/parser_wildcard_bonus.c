/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/11 20:40:06 by ischeini         ###   ########.fr       */
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

static int	add_to_list(t_list **head, char **matches)
{
	t_list	*new_node;
	t_token	*new_token;
	int		i;

	if (!matches || !matches[0])
		return (MSHELL_SUCCESS);
	i = -1;
	while (matches[++i])
	{
		new_token = token_dup(matches[i]);
		if (!new_token)
			break ;
		ft_memset((void *)new_token->mask, 'N', ft_strlen(matches[i]));
		if (i == 0)
		{
			free(((t_token *)head[0]->content)->mask);
			free(((t_token *)head[0]->content)->str);
			free(((t_token *)head[0]->content));
			head[0]->content = new_token;
		}
		else
		{
			new_node = ft_lstnew(new_token);
			if (!new_node)
				break ;
			new_node->next = head[0]->next;
			head[0]->next = new_node;
			head[0] = new_node;
		}
	}
	if (!new_node || !new_token)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

int	parser_wildcard(t_body *msh)
{
	t_list	**head;
	t_list	*crnt;
	char	**matches;
	char	**names;
	
	head = &msh->token_lst;
	names = NULL;
	if (dir_names(&names, msh))
		return (MSHELL_FAILURE);
	while (head[0])
	{
		matches = NULL;
		crnt = head[0];
		if (wildcard(names, &matches, crnt))
			forcend(msh, "malloc", MSHELL_FAILURE);
		if (add_to_list(&crnt, matches))
			forcend(msh, "malloc", MSHELL_FAILURE);
		head = &crnt->next;
		free(matches);
	}
	ft_split_free(names);
	return (MSHELL_SUCCESS);
}
