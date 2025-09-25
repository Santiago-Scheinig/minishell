/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:06:13 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/24 18:35:57 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
static int	mask_quoted(char *str, char *mask_str, char quote)
{
	int	i;

	i = 0;
	mask_str[i] = 'O';
	while (str[++i] && str[i] != quote)
	{
		if (quote == '\'')
			mask_str[i] = 'S';
		else if (quote == '\"')
			mask_str[i] = 'D';
	}
	mask_str[i] = 'O';
	return (i);
}

/**
 * COMMENT PENDING
 */
char	*mask_dup(char *str)
{
	int		i;
	char	quote;
	char	*mask_str;
	
	mask_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!mask_str)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		quote = 0;
		if (str[i] == '\"' && str[i + 1] && ft_strchr(&str[i + 1], '\"'))
			quote = '\"';
		else if (str[i] == '\'' && str[i + 1] && ft_strchr(&str[i + 1], '\''))
			quote = '\'';
		if (quote)
			i += mask_quoted(&str[i], &mask_str[i], quote);
		else if (str[i] == ';' || str[i] == '\\'
			|| str[i] == '\'' || str[i] == '\"')
			mask_str[i] = 'O';
		else
			mask_str[i] = 'N';
	}
	return (mask_str);
}
