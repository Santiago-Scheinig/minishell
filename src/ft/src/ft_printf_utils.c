/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:18:53 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:06:51 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

/**
 * @brief	Prepends a sign or space to a numeric string based on flags.
 *
 *			Checks if the string already has a sign. If flags[1] ('+') is
 *			set, prepends '+' to positive numbers. If flags[2] (' ') is set
 *			and no sign exists, prepends a space. Updates sign and str_len.
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	str		String representing the number to modify.
 * @param	sign	Pointer to int tracking if a sign exists.
 * @param	str_len	Pointer to int storing current string length.
 *
 * @note	Allocates memory for new string if a sign/space is added.
 * @note	Frees the original string if a new string is created.
 *
 * @return	Pointer to updated string with sign/space, or original str.
 */
static	char	*setsign(int *flags, char *str, int *sign, int *str_len)
{
	char	*aux;

	if (!ft_isdigit(str[0]))
		*sign = 1;
	if (flags[1] && !*sign)
	{
		aux = ft_strjoin("+", str);
		if (!aux)
			return (NULL);
		*sign = 1;
		*str_len = ft_strlen(aux);
	}
	else if (flags[2] && !*sign)
	{
		aux = ft_strjoin(" ", str);
		if (!aux)
			return (NULL);
		*sign = 1;
		*str_len = ft_strlen(aux);
	}
	else
		return (str);
	free(str);
	return (aux);
}

/**
 * @brief	Creates a precision-adjusted numeric string with leading zeros.
 *
 *			Allocates a new string with length flags[6] + sign and fills it
 *			with '0'. Copies the first 'sign' characters from str, then
 *			appends the rest of str. Frees the original str and applies
 *			width adjustments via txtwth().
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	sign	Number of characters to preserve at the start of str.
 * @param	str		Original string to apply precision to.
 * @param	str_len	Length of the original string.
 *
 * @note	Allocates memory with ft_calloc and frees the original string.
 * @note	Uses txtwth() to apply field width after precision is applied.
 *
 * @return	Pointer to the new precision-adjusted string, or NULL on failure.
 */
static	char	*getpsc(int *flags, int sign, char *str, int str_len)
{
	char	*pcs;
	int		pcs_len;
	int		i;

	i = -1;
	pcs_len = flags[6] + sign;
	pcs = ft_calloc(pcs_len + 1, sizeof(char));
	if (!pcs)
		return (NULL);
	ft_memset(pcs, '0', pcs_len);
	while (++i < sign)
		pcs[i] = str[i];
	ft_strlcpy(&pcs[pcs_len - str_len + sign], &str[sign], str_len + 1);
	free(str);
	return (txtwth(flags, pcs, pcs_len));
}

/**
 * @brief	Applies field width formatting to a string.
 *
 *			Generates a new string of length equal to the field width
 *			specified in 'flags'. Pads the original string 'str' with
 *			spaces either to the left or right depending on the '-' flag.
 *
 * @param	flags	Array containing printf-like formatting information.
 *					flags[0] is the left-justify flag, flags[5] is width,
 *					flags[7] is the specifier.
 * @param	str		Original string to format.
 * @param	str_len	Length of the original string.
 *
 * @note	Allocates a new string with ft_calloc. Frees the original string.
 * @note	If width <= str_len or specifier is '%', returns a substring copy
 *			of 'str'.
 *
 * @return	Pointer to the newly formatted string, or NULL on allocation
 *			failure.
 */
char	*txtwth(int *flags, char *str, int str_len)
{
	char	*txt;

	if (flags[5] > str_len && flags[7] != '%')
	{
		txt = ft_calloc(flags[5] + 1, sizeof(char));
		if (!txt)
			return (NULL);
		ft_memset(txt, ' ', flags[5]);
		if (flags[0])
			ft_memcpy(txt, str, str_len);
		else
			ft_strlcpy(&txt[flags[5] - str_len], str, str_len + 1);
	}
	else
		txt = ft_substr(str, 0, str_len);
	if (!txt)
		return (NULL);
	free(str);
	return (txt);
}

/**
 * @brief	Applies width, precision, and prefix to a hexadecimal string.
 *
 *			Handles '#' flag by prepending "0x" or "0X" for non-zero values.
 *			Applies precision using getpsc() if flags[6] exceeds string len.
 *			Handles zero-padding with '0' flag when precision is unset.
 *			Finally, adjusts the string width using txtwth().
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	str		Hexadecimal string to format.
 * @param	str_len	Length of the hexadecimal string.
 *
 * @note	Allocates memory for new formatted strings as needed.
 * @note	May free the original str during processing.
 *
 * @return	Pointer to formatted hexadecimal string, or NULL on failure.
 */
char	*hexwth(int *flags, char *str, int str_len)
{
	char	*hex;
	int		sign;

	sign = 0;
	hex = str;
	if (flags[7] != 'p')
	{
		if (flags[3] && ft_strncmp(str, "0", 2))
		{
			hex = ft_ptrdup(str, flags[7]);
			if (!hex)
				return (NULL);
			str_len += 2;
			sign = 2;
		}
		if (flags[6] > str_len - sign)
			return (getpsc(flags, sign, hex, str_len));
		else if (flags[4] && flags[6] == -1 && flags[5] > str_len)
		{
			flags[6] = flags[5] - sign;
			return (getpsc(flags, sign, hex, str_len));
		}
	}
	return (txtwth(flags, hex, str_len));
}

/**
 * @brief	Formats a decimal string with width, precision, and sign flags.
 *
 *			Handles precision zero with value zero by outputting an empty
 *			string or "+" if flags[1] is set. Sets sign for '+' or ' ' flags
 *			using setsign(). Applies precision using getpsc() if needed.
 *			Handles zero-padding when precision is unset and width exceeds
 *			string length. Finally applies width adjustment with txtwth().
 *
 * @param	flags	Array of printf-like formatting flags.
 * @param	str		Decimal string to format.
 * @param	str_len	Length of the decimal string.
 *
 * @note	Allocates memory for new formatted strings as needed.
 * @note	May free the original str during processing.
 *
 * @return	Pointer to formatted decimal string, or NULL on failure.
 */
char	*dgtwth(int *flags, char *str, int str_len)
{
	int		sign;

	sign = 0;
	if (!ft_strncmp(str, "0", 2) && flags[6] == 0)
	{
		free(str);
		if (flags[1])
			return (txtwth(flags, ft_strdup("+"), 1));
		return (txtwth(flags, ft_strdup(""), 0));
	}
	str = setsign(flags, str, &sign, &str_len);
	if (!str)
		return (NULL);
	if (flags[6] > str_len - sign)
		return (getpsc(flags, sign, str, str_len));
	else if (flags[4] && flags[6] == -1 && flags[5] > str_len)
	{
		flags[6] = flags[5] - sign;
		return (dgtwth(flags, str, str_len));
	}
	return (txtwth(flags, str, str_len));
}
