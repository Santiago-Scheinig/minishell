/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_envar_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:58:42 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:47:31 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * @brief	Calculates the length of an environment variable name.
 *
 *			Starts at the '$' sign and counts alphanumeric characters
 *			and underscores, or '?' as a special variable.
 *
 * @param	envar	Pointer to string starting at '$'.
 *
 * @return	Length of the variable name including '$' or '?'.
 * @note	The minimum length is always 1 for the '$' character.
 */
int	envar_len(char *envar)
{
	int	i;

	i = 1;
	if (envar[i] == '?')
		return (++i);
	while (ft_isalnum(envar[i]) || envar[i] == '_')
		i++;
	return (i);
}

/**
 * @brief	Extracts the environment variable name into a new string.
 *
 *			Allocates memory and copies only the variable name (with '$')
 *			from envar to a clean string for getenv-like lookup.
 *
 * @param	envar	String starting at the '$' of the variable name.
 *
 * @return	Pointer to newly allocated string containing the variable.
 */
char	*envar_pathname(char *envar)
{
	char	*new_path;
	int		aux_len;
	int		i;

	i = -1;
	aux_len = envar_len(envar);
	new_path = ft_calloc((aux_len + 1), sizeof(char));
	if (!new_path)
		return (NULL);
	while (++i < aux_len)
		new_path[i] = envar[i];
	return (new_path);
}

/**
 * @brief	Expands a masked environment variable in a string.
 *
 *			Replaces the mask of length len.var at start with a string
 *			of length len.val. Shifts the rest of the string to accommodate.
 *
 * @param	start	Index in mask to start expansion.
 * @param	str		String to insert (value of env variable).
 * @param	mask	String containing original mask.
 * @param	len		Pair struct containing variable and value lengths.
 *
 * @return	Expanded string or original mask if value length is zero.
 * @note	Mallocs a new string if expansion length differs;
 * 			else shifts in-place.
 */
char	*exp_mask(int start, char *str, char *mask, t_envar_pair len)
{
	int		aux_len;
	int		aux_start;
	char	*aux_mask;
	char	*new_mask;

	if (!len.val)
	{
		aux_start = start + len.var;
		aux_len = ft_strlen(&(mask[aux_start]));
		ft_memmove(&(mask[start]), &(mask[aux_start]), aux_len + 1);
		return (mask);
	}
	aux_len = len.val - len.var + ft_strlen(str);
	if (len.val < len.var)
		aux_len = len.var - len.val + ft_strlen(str);
	new_mask = ft_calloc(aux_len + 1, sizeof(char));
	if (!new_mask)
		return (NULL);
	ft_strlcpy(new_mask, mask, aux_len);
	aux_len = ft_strlen(str);
	ft_memset(&new_mask[start], mask[start], aux_len);
	aux_start = start + aux_len;
	aux_mask = &(mask[start + len.var]);
	ft_strlcpy(&new_mask[aux_start], aux_mask, ft_strlen(aux_mask) + 1);
	return (new_mask);
}

/**
 * @brief	Expands an environment variable with a specific value.
 *
 *			If value is NULL, removes the variable mask from str.
 *			Otherwise, allocates a new string with the value inserted.
 *
 * @param	start	Index in str where variable begins.
 * @param	str		Original string containing variable mask.
 * @param	value	Value to replace the mask with.
 *
 * @return	Newly allocated string with expansion, or modified str.
 * @note	Frees original str if allocation occurs.
 */
char	*exp_value(int start, char *str, char *value)
{
	int		aux_len;
	int		aux_start;
	char	*exp_str;
	char	*aux_str;

	if (!value)
	{
		aux_len = envar_len(&(str[start]));
		aux_start = start + aux_len;
		aux_len = ft_strlen(&(str[aux_start])) + 1;
		ft_memmove(&(str[start]), &(str[aux_start]), aux_len);
		return (str);
	}
	aux_len = ft_strlen(value) + ft_strlen(str) + 1;
	exp_str = ft_calloc(aux_len, sizeof(char));
	if (!exp_str)
		return (NULL);
	ft_strlcpy(exp_str, str, aux_len + 1);
	ft_strlcpy(&exp_str[start], value, aux_len + 1);
	aux_str = &(str[start + envar_len(&(str[start]))]);
	aux_start = ft_strlen(exp_str);
	ft_strlcpy(&exp_str[aux_start], aux_str, aux_len);
	return (exp_str);
}

/**
 * @brief	Expands the special $? variable with the last exit number.
 *
 *			Converts exit_no to string and replaces variable at start.
 *
 * @param	start	Index in str where '$?' begins.
 * @param	exit_no	Last command exit code.
 * @param	str		Pointer to original string.
 * @param	mask	Pointer to mask string.
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE on allocation error.
 */
int	exp_exitno(int start, int exit_no, char **str, char **mask)
{
	char	*envar_value;
	char	*aux;

	envar_value = ft_itoa(exit_no);
	aux = exp_value(start, (*str), envar_value);
	if (!aux || envar_mask(start, (*str), envar_value, mask))
		return (MSHELL_FAILURE);
	free(envar_value);
	if ((*str))
		free((*str));
	(*str) = aux;
	return (MSHELL_SUCCESS);
}
