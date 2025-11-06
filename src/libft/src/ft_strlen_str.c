/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:09:52 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 14:12:18 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen_str(char *str, const char *end)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_strncmp(&str[i], end, strlen(end)))
			break;
	return (i);
}