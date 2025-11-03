/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:52:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:15:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isbase(char c, char const *base)
{
	int	i;

	i = 0;
	while (base[i])
		if (base[i++] == c)
			return (true);
	return (false);
}

static int	check_base(char *str, const char *base)
{
	size_t	i;

	i = 0;
	if (!str || !base)
		return (false);
	while (isbase(str[i], base))
		i++;
	if (ft_strlen(str) != i)
		return (false);
	return (true);
}

/**
 * @brief	Converts a string representing a number in a custom base to int.
 *
 * 			Parses the string 'str' as a number in the base defined by the
 *			string 'base', and returns its integer value. Each character in
 *			'str' must belong to the provided base, which defines the symbol
 *			set and its positional value order.
 *
 *			Static helpers:
 *
 *				- isbase():		Checks if a character exists within
 *								the base.
 *
 *				- check_base():	Validates that 'str' contains only base
 *								characters and no invalid symbols.
 *
 *			The conversion multiplies the accumulated result by the base length
 * 			and adds the positional value of each character from left to right.
 *
 * @param	str		String to convert. Must only contain characters from base.
 * @param	base	String containing all valid symbols of the base.
 *
 * @note	If 'str' or 'base' contains invalid input, returns 0.
 * @note	Base length determines the radix (e.g., 2 for binary, 16 for hex).
 *
 * @return	Integer value represented by 'str' in the given base, or 0 on
 *			invalid input.
 */
int	ft_atoi_base(char *str, const char *base)
{
	int	base_lenght;
	int	ans;
	int	i;
	int	index;

	if (!check_base(str, base))
		return (0);
	i = -1;
	ans = 0;
	base_lenght = ft_strlen(base);
	while (str[++i])
	{
		index = -1;
		while (base[++index])
			if (base[index] == str[i])
				break ;
		ans = (ans * base_lenght) + index;
	}
	return (ans);
}
