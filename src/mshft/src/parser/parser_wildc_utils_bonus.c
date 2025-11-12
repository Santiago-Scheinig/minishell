/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildc_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:35:15 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/12 17:48:17 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

/**
 * @brief	Appends a new match string to the matches array.
 *
 * 			Allocates memory to expand the array of strings, copies
 *			existing matches, and adds a new name at the end.
 *
 * @param	matches	Double pointer to the array of matched strings.
 * @param	names	New string to append to the matches array.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
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

/**
 * @brief	Recursively matches a string against a wildcard mask.
 *
 * 			Supports '*' wildcards and literal character matching.
 *			Returns success if the name fits the pattern and mask.
 *
 * @param	str		String pattern containing wildcards.
 * @param	mask	Mask indicating valid wildcard positions ('O').
 * @param	name	String to match against the pattern.
 *
 * @return	MSHELL_SUCCESS if match succeeds, MSHELL_FAILURE otherwise.
 */
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

/**
 * @brief	Validates if a string contains usable wildcard positions.
 *
 * 			Checks that '*' characters in str correspond to 'O' in mask.
 *
 * @param	str		String pattern to validate.
 * @param	mask	Mask indicating which '*' positions are valid.
 *
 * @return	1 if valid wildcards exist, 0 if no wildcards, 0 if invalid.
 */
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

/**
 * @brief	Expands wildcard patterns by matching names in a directory.
 *
 * 			Iterates over the names array, matches each name against
 *			the token's wildcard pattern, and appends matches to matches.
 *
 * @param	names	Array of strings (e.g., filenames) to match against.
 * @param	matches	Double pointer to store matched strings.
 * @param	head	Linked list node containing the current token.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
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
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Copies a directory entry name into a dynamic names array.
 *
 * 			Reallocates the array to add one more entry, then duplicates
 *			the directory name into the new slot.
 *
 * @param	names	Double pointer to the array of names.
 * @param	dir		Name of the directory entry to copy.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on memory error.
 */
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
