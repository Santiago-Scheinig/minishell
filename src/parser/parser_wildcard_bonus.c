/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/04 20:12:56 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include <dirent.h> //bonus

static int	copy_name(char ***names, char *dir)
{
	char	**tmp;
	int		i;

	i = ft_arrlen(names[0]);
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
	struct dirent	*copy;
	DIR				*dir;

	dir = opendir(".");
	if (!dir)
		forcend(msh, "opendir", MSHELL_FAILURE);
		return (MSHELL_FAILURE);
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
			return (MSHELL_FAILURE);
		}
	}
	closedir(dir);
	return (MSHELL_SUCCESS);
}

static int	check_wildcard(t_body *msh, t_list *head)
{
	t_token	*token;
	char	**names;
	char	**str;
	
	names = NULL;
	if (dir_names(&names, msh))
		return (MSHELL_FAILURE);
	token = (t_token *)head->content;
	if (!ft_strchr(token->str, '*'))
		return (MSHELL_SUCCESS);
	str = ft_split(token->str, '*');
	
}

void	parser_wildcard(t_body *msh)
{
	t_list	*head;
	
	head = msh->token_lst;
	while (head)
	{
		if (check_wildcard(msh, head))
	}
}