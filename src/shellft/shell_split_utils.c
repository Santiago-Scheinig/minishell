/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 20:22:22 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/03 21:57:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * COMMENT PENDING
 */
int	is_divisor(char *str)
{
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	if (!ft_strncmp(str, " ", 1))
		return (END);
	return (WORD);
}

/**
 * COMMENT PENDING
 */
int	get_token_type(char *str)
{
	if (!str || !str[0])
		return (END);
	if (!ft_strncmp(str, "|", 1))
		return (PIPE);
	if (!ft_strncmp(str, ">>", 2))
		return (REDIR_APPEND);
	if (!ft_strncmp(str, "<<", 2))
		return (HEREDOC);
	if (!ft_strncmp(str, "<", 1))
		return (REDIR_IN);
	if (!ft_strncmp(str, ">", 1))
		return (REDIR_OUT);
	return (WORD);
}

/**
 * Frees every pointer on an ARRAY of STRINGS and the ARRAY pointer, even
 * if it's not NULL terminated.
 * @param wrdstr The ARRAY of STRINGS to free.
 * @param index The amount of STRINGS to free inside of the array.
 */
void	*memfree(char **wrdstr, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(wrdstr[i]);
		i++;
	}
	free(wrdstr);
	return (NULL);
}

/**
 * Returns the proper length of the operator type sent as argument.
 * @param type The T_TOKEN_TYPE that describers the operator type.
 * @return The lenght of the operator type.
 * @note If the operator isn't valid, returns 0.
 */
int	operator_len(int type)
{
	if (type == PIPE || type == REDIR_IN || type == REDIR_OUT)
		return (1);
	if (type == HEREDOC || type == REDIR_APPEND)
		return (2);
	return (0);
}

/**
 * Counts the lenght of the first word on a STRING, until it reaches a
 * space, shell operator or '\0'
 * @param s The STRING where to count the lenght of the first word.
 * @return An INT with the lenght of the word.
 * @note This function will ignore any divisor coincidences that happen 
 * to be inside of single and double quotes (as long they open and close).
 */
int	word_len(const char *s)
{
	char	*tmp;
	char	quote;
	int		i;

	i = -1;
	tmp = NULL;
	while (s[++i] && !is_divisor((char *) &s[i]))
	{
		if (s[i] == '\'')
		{
			tmp = ft_strchr(&s[i + 1], '\'');
			quote = '\'';
		}
		else if (s[i] == '\"')
		{
			tmp = ft_strchr(&s[i + 1], '\"');
			quote = '\"';
		}
		if (tmp && s[++i])
			while (s[i] && s[i] != quote)
				i++;
		tmp = NULL;
	}
	return (i);
}
