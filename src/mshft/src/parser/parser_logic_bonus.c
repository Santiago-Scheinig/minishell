/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:11:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 11:11:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

static int logic_parser_parenthesis(t_list *head_logic)
{
	t_logic		*aux;
	const char	*errmsh = "msh: syntax error: unclosed parenthesis\n";
	int			parenthesis_count;

	parenthesis_count = 0;
	while (head_logic)
	{
		aux = (t_logic *)head_logic->content;
		if (aux->type == LPARENTHESIS)
			parenthesis_count++;
		else if (aux->type == RPARENTHESIS)
		{
			parenthesis_count--;
			if (parenthesis_count < 0)
				return (ft_fprintf(STDERR_FILENO, "%s", errmsh));
		}
		head_logic = head_logic->next;
	}
	if (parenthesis_count != 0)
		return (ft_fprintf(STDERR_FILENO, "%s", errmsh));
	return (MSHELL_SUCCESS);
}

static int logic_parser_operators(t_list *head_logic, t_body *msh)
{
	t_logic	*aux;
	char	*current;
	int		prev_type;

	prev_type = UNSET;
	while (head_logic)
	{
		aux = (t_logic *)head_logic->content;
		current = aux->str;
		if (aux->type == AND || aux->type == OR)
		{
			if (prev_type != INPUT && prev_type != RPARENTHESIS)
				return (shell_parserr(current, msh));
			prev_type = aux->type;
		}
		else
			prev_type = aux->type;
		head_logic = head_logic->next;
	}
	if (prev_type == AND || prev_type == OR)
		return (shell_parserr(NULL, msh));
	return (MSHELL_SUCCESS);
}

static int	logic_tknizer(char **split, t_list **head_logic)
{
	t_list	*new_node;
	t_logic	*new_logic;
	int		i;

	i = -1;
	while (split[++i])
	{
		new_logic = shell_newlogic(split[i]);
		if (!new_logic)
		{
			ft_split_free(&split[i]);
			return (MSHELL_FAILURE);
		}
		new_node = ft_lstnew(new_logic);
		if (!new_node)
		{
			free(new_logic);
			ft_split_free(&split[i]);
			return (MSHELL_FAILURE);
		}
		ft_lstadd_back(head_logic, new_node);
	}
	free(split);
	return (MSHELL_SUCCESS);
}

static int	logic_syntax(char **split, t_body *msh)
{
	t_list		*head_logic;

	head_logic = NULL;
	if (logic_tknizer(split, &head_logic))
	{
		ft_lstclear(&head_logic, shell_dellogic);
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	}
	if (logic_parser_parenthesis(head_logic)
	|| logic_parser_operators(head_logic, msh))
	{
		ft_lstclear(&head_logic, shell_dellogic);
		return (MSHELL_FAILURE);
	}
	ft_lstclear(&head_logic, shell_dellogic);
	return (MSHELL_SUCCESS);
}

int	logic_parser(char **logic_input, t_body *msh)
{
	char		*input;
	char		**split;
	const char	*l_base[] = {
		"(", ")", "&&", "||", NULL,
	};

	input = input_reader(msh);
	if (!input[0])
	{
		free(input);
		return (MSHELL_FAILURE);
	}
	split = ft_split_iqbase(input, ' ', l_base);
	if (!split)
	{
		free(input);
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	}
	if (logic_syntax(split, msh))
	{
		free(input);
		return (MSHELL_FAILURE);
	}
	(*logic_input) = input;
	return (MSHELL_SUCCESS);
}
