/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:29:32 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:18:04 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_BONUS_H
# define MSH_EXE_BONUS_H

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include "shell_std.h"

/*--------------------------------------------------------------------------*/
/*----------------------------EXECUTION LIBRARY-----------------------------*/
/*--------------------------------------------------------------------------*/

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
int			logic_execution(char *input, t_body *msh);

/**
 * @brief	Extracts the portion of input to the right of a logical operator.
 *
 *			Finds the last occurrence of the operator ("&&" or "||") and
 *			returns a newly allocated substring containing the commands
 *			to the right of it.
 *
 * @param	input		Input command string.
 * @param	operator	Logical operator to split at.
 * @param	msh			Shell context, used for error handling on malloc.
 *
 * @return	Newly allocated string with right-side commands.
 */
char		*logic_input_left(char *input, const char *operator, t_body *msh);

/**
 * @brief	Extracts the portion of input to the right of a logical operator.
 *
 *			Finds the last occurrence of the operator ("&&" or "||") and
 *			returns a newly allocated substring containing the commands
 *			to the right of it.
 *
 * @param	input		Input command string.
 * @param	operator	Logical operator to split at.
 * @param	msh			Shell context, used for error handling on malloc.
 *
 * @return	Newly allocated string with right-side commands.
 */
char		*logic_input_right(char *input, const char *operator, t_body *msh);

/**
 * @brief	Executes a subshell for a command sequence in parentheses.
 *
 *			Forks a new process, extracts the command inside '(' and ')',
 *			executes it via logic_execution(), and updates msh->input_result
 *			based on the exit status.
 *
 * @param	input	Input command string containing parentheses.
 * @param	msh		Shell context containing environment and state.
 *
 * @return	MSHELL_SUCCESS after execution; updates msh->exit_no and
 *			msh->input_result accordingly.
 */
int			logic_subshell(char	*input, t_body *msh);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif