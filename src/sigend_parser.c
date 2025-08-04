/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigend_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 21:35:32 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/04 18:57:15 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigend(t_body *minishell, int errno)
{
	if (minishell || errno)
		return ;
	return;	
}