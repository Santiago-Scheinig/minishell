/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_argjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:13:33 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/04 22:27:07 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief   Joins all command-line arguments into a single string separated
 *          by a given character.
 *
 *          The function concatenates all strings in @p argv, starting from
 *          index 1 (skipping argv[0], usually the program name), inserting
 *          the separator character @p c between each argument. The result is
 *          dynamically allocated and null-terminated.
 *
 * @param   argv    Array of strings, typically the argument vector (NULL-
 *                  terminated).
 * @param   c       Character used as a separator between concatenated
 *                  arguments.
 *
 * @note    The returned string includes a trailing separator at the end.
 *          If this is not desired, additional handling may be needed.
 *
 * @note    The function assumes @p argv is non-NULL and properly terminated.
 *          Passing NULL or a malformed array results in undefined behavior.
 *
 * @warning The call to ft_strlcat() with &c as argument may be unsafe since
 *          @p c is not null-terminated. This could cause undefined behavior
 *          depending on the implementation of ft_strlcat(). Consider using
 *          a temporary buffer of size 2 (e.g., "{c, '\0'}") to ensure safe
 *          concatenation.
 *
 * @return  Pointer to the newly allocated joined string on success, or NULL
 *          if memory allocation fails.
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
