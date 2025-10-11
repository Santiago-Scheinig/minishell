/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_utils_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 15:35:15 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/05 15:36:14 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

static 

char	**ft_split_wildcard(char **mask, char **str, char c)
{
	char	**wrdstr;
	int		w;
	int		i;

	if (str[0] == 0)
		return (ft_calloc(1, sizeof(char *)));
	i = 0;
	w = wrdcount(mask, str, c);
	wrdstr = (char **) malloc((w + 1) * sizeof(char *));
	if (!wrdstr)
		return (NULL);
	while (i < w)
	{
		while (str[0] == c)
			str = ft_strchr(str, c) + 1;
		wrdstr[i] = ft_substr(str, 0, ft_wrdlen(str, c));
		if (!wrdstr[i])
			return (ft_memfree(wrdstr, i), NULL);
		str = ft_strchr(str, c) + 1;
		i++;
	}
	wrdstr[i] = (void *)(0);
	return (wrdstr);
}