/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:10:38 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 10:33:05 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

void	shell_dellogic(void *content)
{
	t_logic	*logic;

	logic = (t_logic *)content;
	if (logic)
	{
		if (logic->str)
			free(logic->str);
		free(logic);
	}
}

t_logic	*shell_newlogic(char *str)
{
	t_logic	*logic;

	logic = malloc(sizeof(t_logic));
	if (!logic)
		return (NULL);
	logic->str = str;
	if (ft_strncmp(str, "(", 1) == 0)
		logic->type = LPARENTHESIS;
	else if (ft_strncmp(str, ")", 1) == 0)
		logic->type = RPARENTHESIS;
	else if (ft_strncmp(str, "&&", 2) == 0)
		logic->type = AND;
	else if (ft_strncmp(str, "||", 2) == 0)
		logic->type = OR;
	else
		logic->type = INPUT;
	return (logic);
}