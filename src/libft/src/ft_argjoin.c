/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:13:33 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:22:20 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Joins command-line arguments into a single string with a separator.
 *
 * 			Concatenates all strings from argv starting at index 1, inserting
 * 			the character 'c' between each argument. The resulting string 
 * 			always ends with the separator character. Memory is dynamically 
 * 			allocated for the result using ft_calloc().
 *
 * @param	argv	Array of argument strings, terminated by NULL.
 * @param	c		Character used as a separator between arguments.
 *
 * @note	The first element argv[0] is skipped (commonly the program name).
 * @note	The returned string ends with the separator and must be freed by
 *			the caller.
 *
 * @return	Newly allocated string containing the concatenated arguments with
 *			separators, or NULL if allocation fails.
 */
char	*ft_argjoin(char **argv, char c)
{
	char	*str;
	int		i;
	size_t	str_len;

	i = 0;
	str_len = 0;
	while (argv[++i])
		str_len += ft_strlen(argv[i]);
	str_len += i;
	str = ft_calloc(str_len + 1, sizeof(char));
	i = 0;
	while (argv[++i])
	{
		ft_strlcat(str, argv[i], str_len);
		ft_strlcat(str, &c, str_len);
	}
	return (str);
}
