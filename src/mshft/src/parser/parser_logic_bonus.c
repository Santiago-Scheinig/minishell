/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:11:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/05 15:16:01 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

static int input_parser(char **split, t_body *msh)
{
	t_list	*new_node;
	t_logic	*new_logic;
	int		i;

	i = -1;
	msh->head_logic = NULL;
	while (split[++i])
	{
		new_logic = shell_newlogic(split[i]);
		if (!new_logic)
		{
			ft_free_split(split);
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		}
		new_node = ft_lstnew(new_logic);
		if (!new_node)
		{
			free(new_logic);
			ft_free_split(split);
			shell_forcend(MSHELL_FAILURE, "malloc", msh);
		}
		ft_lstadd_back(&(msh->head_logic), new_node);
	}
	return (MSHELL_SUCCESS);//return (parser_logic_syntax(msh->head_logic));
}

int	logic_input(t_body *msh)
{
	char	*l_base[]{
		"(", ")", "&&", "||", NULL,
	};
	char	**split;
	char	*input;

	input = input_reader(msh);
	split = ft_split_iqbase(input, ' ', l_base);
	if (!split)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	if (input_parser(split, msh))
		return (MSHELL_FAILURE);
	ft_free_split(split);
	return (MSHELL_SUCCESS);
}