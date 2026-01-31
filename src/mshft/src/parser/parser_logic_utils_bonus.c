/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 10:10:38 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:39:24 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

/**
 * @brief	Frees a t_logic structure and its contents.
 *
 *			Frees the dynamically allocated string inside t_logic,
 *			then frees the t_logic struct itself.
 *
 * @param	content	Pointer to a t_logic struct to free.
 */
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

/**
 * @brief	Creates a new t_logic structure from a string token.
 *
 *			Allocates a t_logic struct, sets the string pointer, and
 *			determines the token type (LPARENTHESIS, RPARENTHESIS,
 *			AND, OR, or INPUT) based on the string.
 *
 * @param	str	String token to wrap in t_logic.
 *
 * @return	Pointer to newly allocated t_logic, or NULL on malloc failure.
 */
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
