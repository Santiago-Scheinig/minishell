/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 18:32:09 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:07:11 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief	Reads all lines from a file descriptor into a linked list.
 *
 * 			Reads lines from the file descriptor 'fd' using get_next_line
 *			and stores each line as a new node in the linked list 'lines'.
 *			Frees all allocated memory and returns -1 if allocation fails.
 *
 * @param	lines	Pointer to the pointer of the first node of the list.
 * @param	fd		File descriptor to read from.
 *
 * @return	Number of lines read, or -1 on allocation failure.
 */
int	ft_read_file(t_list **lines, int fd)
{
	t_list	*new;
	char	*str;
	int		count;

	count = 0;
	str = ft_get_next_line(fd);
	while (str)
	{
		new = ft_lstnew(str);
		if (!new)
		{
			ft_lstclear(lines, free);
			return (-1);
		}
		ft_lstadd_back(lines, new);
		if (!(*lines))
		{
			ft_lstclear(lines, free);
			return (-1);
		}
		count++;
		str = ft_get_next_line(fd);
	}
	return (count);
}
