/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:52:46 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:28:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Checks if a character is part of a given base string.
 *
 *          Iterates through the @p base string to see if @p c matches any
 *          character. Used to validate characters when converting numbers
 *          from a specific base.
 *
 * @param   c       The character to check.
 * @param   base    The string representing allowed digits for the base.
 *
 * @return  Returns 1 if @p c is found in @p base, 0 otherwise.
 */
static int	ft_isbase(char c, char const *base)
{
	int	i;

	i = 0;
	while (base[i])
		if (base[i++] == c)
			return (1);
	return (0);
}

/**
 * @brief   Validates that a string contains only characters from a given base.
 *
 *          Iterates through @p str and checks that every character exists in
 *          the @p base string using ft_isbase. If any character is not part
 *          of the base, the string is considered invalid.
 *
 * @param   str     The string to validate.
 * @param   base    The string representing allowed digits for the base.
 *
 * @return  Returns 0 if all characters in @p str are in @p base.
 *          Returns 1 if @p str contains invalid characters or if
 *          @p str or @p base is NULL.
 */
static int	ft_check_base(char *str, const char *base)
{
	size_t	i;

	i = 0;
	if (!str || !base)
		return (1);
	while (ft_isbase(str[i], base))
		i++;
	if (ft_strlen(str) != i)
		return (1);
	return (0);
}

/**
 * @brief   Converts a string representing a number in a custom base to an int.
 *
 *          Iterates through the string @p str, finds each character's index
 *          in the provided @p base, and accumulates the result as a decimal
 *          integer. Supports arbitrary bases defined by the order of
 *          characters in @p base.
 *
 * @param   str     The string representing the number in the given base.
 * @param   base    A string of characters representing the digits of the base.
 *                  Each character must be unique.
 *
 * @note    If the input string contains characters not in @p base, or if
 *          the base is invalid, the function returns 0.
 * @note    This function does not handle negative numbers.
 *
 * @return  The integer value of the string in base 10. Returns 0 on invalid
 *          input or base.
 */
int	ft_atoi_base(char *str, const char *base)
{
	int	base_lenght;
	int	ans;
	int	i;
	int	index;

	if (ft_check_base(str, base))
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
