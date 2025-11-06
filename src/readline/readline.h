/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 17:46:19 by ischeini          #+#    #+#             */
/*   Updated: 2025/11/06 17:46:40 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_readline(const char *prompt);

int	handlers(char **buffer, int *new_size, int *pos, char chr);

