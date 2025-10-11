/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/05 15:35:24 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include <dirent.h> //bonus

static int	copy_name(char ***names, char *dir)
{
	char	**tmp;
	int		i;

	i = ft_arrlen((const void **)names[0]);
	tmp = ft_realloc(names[0], sizeof(char *) * (i + 2), sizeof(char *) * i);
	if (!tmp)
		return (MSHELL_FAILURE);
	names[0] = tmp;
	names[0][i] = ft_strdup(dir);
	if (!names[0][i])
		return (MSHELL_FAILURE);
	names[0][i + 1] = NULL;
	return (MSHELL_SUCCESS);
}

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


static int	check_wildcard(char **names, t_body *msh, t_list *head)
{
	t_token	*token;
	char	**str;

	str = NULL;
	token = (t_token *)head->content;
	if (!token->str)
		return (MSHELL_SUCCESS);
	if (!ft_strchr(token->str, '*'))
		return (MSHELL_SUCCESS);
	str = ft_split_wildcard(token->mask, token->str, '*');
	if (!str)
		forcend(msh, "malloc", MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

void	parser_wildcard(t_body *msh)
{
	t_list	*head;
	char	**names;
	
	head = msh->token_lst;
	names = NULL;
	while (head)
	{
		if (dir_names(&names, msh))
			return (MSHELL_FAILURE);
		check_wildcard(names[0], msh, head);
		head = head->next;
	}
}