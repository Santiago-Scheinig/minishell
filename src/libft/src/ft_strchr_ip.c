/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr_ip.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 12:53:18 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 12:54:14 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int is_parenthesis(char c)
{
	int	i;

	i = -1;
	if (c == '(')
		return (')');
	return (0);
}

char	*ft_strchr_ip(const char *s, char c)
{
	char	*tmp;
	int		i;
	int		parenthesis;

	i = 0;
	tmp = NULL;
	while (s[i] && s[i] != c)
	{
		parenthesis = is_parenthesis(s[i]);
		if (parenthesis && s[i + 1])
			tmp = ft_strchr(&s[i + 1], parenthesis);
		if (tmp)
		{
			s = tmp;
			i = 0;
			tmp = NULL;
		}
		i++;
	}
	if (!s[i])
		return (NULL);
	return ((char *) &s[i]);
}