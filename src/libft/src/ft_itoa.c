/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:44:50 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 04:49:12 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	char	*ft_swap(char *str)
{
	char	swap;
	int		len;
	int		i;

	i = 0;
	if (str[0] == '-')
		i++;
	len = ft_strlen(str);
	while (i < len - 1)
	{
		swap = str[len - 1];
		str[len - 1] = str[i];
		str[i] = swap;
		i++;
		len--;
	}
	return (str);
}

static	char	*ft_setstr(int n, int len)
{
	char	*str;

	if (n < 0)
	{
		str = ft_calloc(len + 2, 1);
		if (!str)
			return (NULL);
		str[0] = '-';
	}
	else
	{
		str = ft_calloc(len + 1, 1);
		if (!str)
			return (NULL);
	}
	return (str);
}

static	char	*ft_min(void)
{
	char	*str;

	str = ft_itoa(INT_MIN + 1);
	if (!str)
		return (NULL);
	str[ft_strlen(str) - 1]++;
	return (str);
}

static	int	ft_nbrlen(int n)
{
	int	count;

	n = n * 1;
	count = 1;
	if (n < 0)
		n *= -1;
	while (n >= 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

/**
 * @brief	Converts an integer to a null-terminated string.
 *
 * 			Allocates memory and creates a string representing the integer
 *			'n'. Handles negative numbers and the special case of INT_MIN.
 *
 *			Static helpers:
 *
 *				- ft_swap():	Reverses the string, handling negative sign.
 *
 *				- ft_setstr():	Allocates the string and sets '-' if needed.
 *
 *				- ft_min():		Handles INT_MIN conversion safely.
 *
 *				- ft_nbrlen():	Calculates the number of digits in 'n'.
 *
 * @param	n	Integer to convert.
 *
 * @note	Memory is allocated for the returned string. Caller must free it.
 *
 * @return	Pointer to the newly allocated string representing 'n', or NULL
 *			on allocation failure.
 */
char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		i;

	i = 0;
	len = ft_nbrlen(n);
	if (n == INT_MIN)
		return (ft_min());
	str = ft_setstr(n, len);
	if (!str)
		return (NULL);
	if (n < 0)
	{
		n *= -1;
		i++;
	}
	while (n >= 10)
	{
		str[i++] = (n % 10) + 48;
		n /= 10;
	}
	str[i] = n + 48;
	return (ft_swap(&str[0]));
}
