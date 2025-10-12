/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildc_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:35:15 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/12 15:08:56 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static int	add_match(char ***matches, char *names)
{
	char	**aux;
	int		len;
	int		i;

	len = ft_arrlen((const void **)matches[0]);
	aux = malloc(sizeof(char *) * (len + 2));
	if (!aux)
		return (MSHELL_FAILURE);
	i = -1;
	while (++i < len)
		aux[i] = matches[0][i];
	aux[len] = ft_strdup(names);
	if (!aux[len])
	{
		ft_split_free(aux);
		return (MSHELL_FAILURE);
	}
	aux[len + 1] = NULL;
	if (matches[0])
		free(matches[0]);
	matches[0] = aux;
	return (MSHELL_SUCCESS);
}

static int	wild_match(char *str, char *mask, char *name)
{
	if (!str[0])
	{
		if (!name[0])
			return (MSHELL_SUCCESS);
		return (MSHELL_FAILURE);
	}
	if (str[0] == '*' && mask[0] == 'O')
	{
		if (!wild_match(str + 1, mask + 1, name))
			return (MSHELL_SUCCESS);
		if (name[0] && !wild_match(str, mask, name + 1))
			return (MSHELL_SUCCESS);
		return (MSHELL_FAILURE);
	}
	if (str[0] == name[0])
		return (wild_match(str + 1, mask + 1, name + 1));
	return (MSHELL_FAILURE);
}

static int	valid_str(char *str, char *mask)
{
	int	valid;
	int	i;

	valid = 1;
	i = -1;
	if (!ft_strchr(str, '*'))
		return (0);
	while (str && str[++i])
	{
		if (str[i] == '*' && mask[i] != 'O')
			valid = 0;
		if (str[i] == '*' && mask[i] == 'O')
			valid = 1;
	}
	return (valid);
}

int	wildcard(char **names, char ***matches, t_list *head)
{
	t_token	*token;
	int		i;

	token = (t_token *)head->content;
	if (!valid_str(token->str, token->mask))
		return (MSHELL_SUCCESS);
	i = -1;
	while (names && names[++i])
	{
		if (!wild_match(token->str, token->mask, names[i]))
			if (add_match(matches, names[i]))
				return (MSHELL_FAILURE);
	}
	if (!matches[0])
		return (MSHELL_SUCCESS);
	return (MSHELL_SUCCESS);
}

int	copy_name(char ***names, char *dir)
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
