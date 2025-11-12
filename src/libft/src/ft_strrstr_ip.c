/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrstr_ip.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:34:07 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:01:02 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	match_at(const char *big, const char *lit, int i)
{
	int	j;

	j = 0;
	while (lit[j] && big[i + j])
	{
		if (big[i + j] != lit[j])
			return (0);
		j++;
	}
	return (lit[j] == '\0');
}

static void	update_state(char c, int *par, int *sq, int *dq)
{
	if (c == ')' && *sq == 0 && *dq == 0)
		(*par)++;
	else if (c == '(' && *par > 0 && *sq == 0 && *dq == 0)
		(*par)--;
	else if (c == '\'' && *dq == 0)
		*sq = !(*sq);
	else if (c == '"' && *sq == 0)
		*dq = !(*dq);
}

/**
 * @brief	Finds the last occurrence of a substring ignoring quotes
 *			and parentheses.
 *
 *			Searches backward in 'big' up to 'len' characters,
 *			skipping sections inside parentheses or quotes. Returns
 *			the last occurrence found outside ignored segments.
 *
 *			Static helpers:
 *
 *				- match_at():		Checks if substring matches at index.
 *				- update_state():	Tracks parentheses and quote state.
 *
 * @param	big			String to search in.
 * @param	little		Substring to find.
 * @param	len			Maximum number of characters to search.
 *
 * @note	Returns NULL if 'little' is empty or not found outside
 *			ignored segments.
 *
 * @return	Pointer to last match of 'little' in 'big', or
 *			NULL if not found.
 */
char	*ft_strrstr_ip(const char *big, const char *little, size_t len)
{
	int		i;
	int		par;
	int		sq;
	int		dq;
	char	*last;

	if (!big || !little || !*little)
		return (NULL);
	i = len - 1;
	par = 0;
	sq = 0;
	dq = 0;
	last = NULL;
	while (i >= 0)
	{
		update_state(big[i], &par, &sq, &dq);
		if (par == 0 && sq == 0 && dq == 0
			&& match_at(big, little, i))
			last = (char *)&big[i];
		i--;
	}
	return (last);
}
