/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 13:09:58 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/02 21:04:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static	int	ft_isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

static	int	ft_islow(int c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	return (0);
}

/**
 * COMMENT PENDING
 */
int	ft_isalpha(int c)
{
	if (ft_isupper(c) || ft_islow(c))
		return (1);
	return (0);
}
