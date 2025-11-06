/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr_ip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:57:51 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 15:34:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int jump_parenthesis(const char *str, int i)
{
	if (str[i] == '(')
	{
		while (str[i] && str[i] != ')')
			i++;
		return (++i);
	}
	return (i);
}

static int	jump_quotes(const char *str, int i)
{
	int quote;

	quote = 0;
	if (str[i] == '\'')
		quote = '\'';
	if (str[i] == '\"')
		quote = '\"';
	if (quote)
	{
		i++;
		while (str[i] && str[i] != quote)
			i++;
	}
	return (i);
}

char	*ft_strnstr_ip(const char *big, const char *little, size_t len)
{
	size_t	i;
	int		little_len;

	if (!big)
		return (NULL);
	little_len = ft_strlen(little);
	if (!little_len)
		return ((char *) big);
	i = 0;
	while (i < len)
	{
		if (!big[i])
			return (NULL);
		i = jump_quotes(big, i);
		i = jump_parenthesis(big, i);
		if (big[i] == little[0] && len >= little_len + i)
		{
			if (ft_strncmp(&big[i], &little[0], little_len) == 0)
				return ((char *) &big[i]);
		}
		i++;
	}
	return (NULL);
}
