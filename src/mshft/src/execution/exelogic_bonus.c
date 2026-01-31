/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exelogic_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:28:31 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:17:29 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"
#include "msh_psr_bonus.h"
#include "msh_exe.h"
#include "msh_exe_bonus.h"

/**
 * @brief	Parses and executes a shell input string.
 *
 *			Performs tokenization, variable expansion, wildcard
 *			expansion, command parsing, and execution. Waits for
 *			child processes if needed and handles cleanup.
 *
 * @param	input	Input command line string.
 * @param	msh		Shell context containing environment and state.
 *
 * @return	MSHELL_SUCCESS if execution succeeds, MSHELL_FAILURE
 *			or exit code on error.
 */
static int	parser(char *input, t_body *msh)
{
	char	**split;

	msh->input_result = MSHELL_SUCCESS;
	if (parser_input(input, &split, msh))
		return (MSHELL_FAILURE);
	if (parser_token(split, msh))
	{
		if (!msh->interactive && msh->exit_no == MSHELL_MISSUSE)
			shell_forcend(msh->exit_no, NULL, msh);
		if (msh->exit_no == MSHELL_FAILURE)
			shell_forcend(msh->exit_no, msh->exit_ft, msh);
		return (msh->exit_no);
	}
	parser_envar(msh);
	parser_wildcard(msh);
	parser_cmds(msh);
	if (execution(msh))
		return (MSHELL_FAILURE);
	if (msh->childs_pid)
		waitexec(msh);
	shell_cleanup(false, msh);
	if (msh->input_result == MSHELL_FAILURE)
		return (MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

/**
 * @brief	Determines the highest-priority logical operator in input.
 *
 *			Searches for "||" and "&&" operators, returning the one
 *			that occurs last (highest evaluation priority).
 *
 * @param	input	Command line string to search.
 *
 * @return	Pointer to "||" or "&&" in the input, or NULL if none.
 */
const char	*get_operator_priority(char *input)
{
	char	*operator_or;
	char	*operator_and;

	operator_or = ft_strrstr_ip(input, "||", ft_strlen(input));
	operator_and = ft_strrstr_ip(input, "&&", ft_strlen(input));
	if (operator_or && operator_and)
	{
		if (operator_or > operator_and)
			return ("||");
		else
			return ("&&");
	}
	if (!operator_or && operator_and)
		return ("&&");
	if (operator_or && !operator_and)
		return ("||");
	return (NULL);
}

/**
 * @brief	Executes a command line with logical operators handling.
 *
 *			Recursively executes left and right sides of "&&" and "||"
 *			operators according to shell logic rules. Handles subshells
 *			and signal interruptions.
 *
 * @param	input	Input command line string.
 * @param	msh		Shell context containing environment and state.
 *
 * @note	Signals (SIGINT) during input reading are handled via
 *			shell_sigint_read().
 *
 * @return	MSHELL_SUCCESS on success, MSHELL_FAILURE, or a special
 *			signal handling code.
 */
int	logic_execution(char *input, t_body *msh)
{
	char		*left_input;
	const char	*operator = NULL;

	if (shell_sigint_read(msh))
	{
		free(input);
		return (MSHELL_SIG_HANDLR);
	}
	operator = get_operator_priority(input);
	if (operator)
	{
		left_input = logic_input_left(input, operator, msh);
		logic_execution(left_input, msh);
		if ((!ft_strncmp(operator, "||", 2) && msh->input_result)
			|| (!ft_strncmp(operator, "&&", 2) && !msh->input_result))
			logic_execution(logic_input_right(input, operator, msh), msh);
		free(input);
		return (MSHELL_SUCCESS);
	}
	else if (ft_strchr_iq(input, '('))
		return (logic_subshell(input, msh));
	return (parser(input, msh));
}
