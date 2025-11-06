/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildc_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 17:56:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 14:01:16 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

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
