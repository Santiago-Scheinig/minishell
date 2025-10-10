/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:53:51 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 05:09:50 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief	Applies a function to each character of a string.
 *
 * 			Iterates over the string 's' and applies the function 'f' to
 *			each character. The function receives the character's index and
 *			a pointer to the character itself.
 *
 * @param	s	String to iterate over.
 * @param	f	Function to apply to each character and its index.
 *
 * @note	The function 'f' can modify the characters in the string directly.
 */
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	int		i;

	i = 0;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
