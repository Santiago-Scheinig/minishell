/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logic_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 17:11:59 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 17:48:09 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"

/**
 * @brief	Checks for balanced parentheses in a token list.
 *
 *			Increments count for '(' and decrements for ')'. Prints
 *			error if closing parenthesis occurs without matching open
 *			or if any are left unmatched at the end.
 *
 * @param	head_logic	Linked list of logic tokens.
 *
 * @return	MSHELL_SUCCESS if balanced, error code otherwise.
 */
static int	logic_parser_parenthesis(t_list *head_logic)
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

/**
 * @brief	Validates logical operator placement in token list.
 *
 *			Ensures '&&' and '||' only follow an input token or ')'.
 *			Also ensures input does not end with an operator.
 *
 * @param	head_logic	Linked list of logic tokens.
 * @param	msh			Main shell structure for error handling.
 *
 * @return	MSHELL_SUCCESS if syntax valid, error code otherwise.
 */
static int	logic_parser_operators(t_list *head_logic, t_body *msh)
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

/**
 * @brief	Tokenizes a split string into logic token linked list.
 *
 *			Creates a t_logic struct for each string, wraps it in
 *			a t_list node, and appends to head_logic.
 *
 * @param	split		NULL-terminated array of token strings.
 * @param	head_logic	Pointer to head of token list to populate.
 *
 * @return	MSHELL_SUCCESS if successful, MSHELL_FAILURE on malloc error.
 */
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

/**
 * @brief	Performs syntax checks on logical token array.
 *
 *			Calls logic_tknizer(), then validates parentheses and
 *			operator placement. Frees token list on failure.
 *
 * @param	split	Array of split strings to parse.
 * @param	msh		Main shell structure for error handling.
 *
 * @return	MSHELL_SUCCESS if syntax valid, MSHELL_FAILURE otherwise.
 */
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

/**
 * @brief	Main logic parser for input with &&, ||, and parentheses.
 *
 *			Splits input based on operators and spaces, then performs
 *			syntax validation.
 *
 * @param	input	Input string from user.
 * @param	msh		Main shell structure.
 *
 * @return	MSHELL_SUCCESS if input is syntactically correct,
 *			MSHELL_FAILURE otherwise.
 */
int	logic_parser(char *input, t_body *msh)
{
	int			i;
	char		**split;
	const char	*l_base[] = {
		"(", ")", "&&", "||", NULL,
	};

	i = -1;
	split = ft_split_iqbase(input, ' ', l_base);
	if (!split)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	if (logic_syntax(split, msh))
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}
