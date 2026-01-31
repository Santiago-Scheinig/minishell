/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exelogic_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:13:45 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/12 18:17:21 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_exe.h"
#include "msh_exe_bonus.h"

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
int	logic_subshell(char	*input, t_body *msh)
{
	char	*sub_input;
	int		subshell_pid;
	int		status;

	subshell_pid = fork();
	if (!subshell_pid)
	{
		input = ft_strchr(input, '(') + 1;
		sub_input = ft_substr(input, 0, ft_strlen_chr(input, ')'));
		logic_execution(sub_input, msh);
		if (msh->input_result)
			exit(MSHELL_FAILURE);
		exit(MSHELL_SUCCESS);
	}
	if (waitpid(subshell_pid, &status, 0) == -1)
		perror("msh: waitpid");
	free(input);
	msh->exit_no = check_status(status, STDERR_FILENO, 0, 0);
	if (msh->exit_no)
		msh->input_result = MSHELL_FAILURE;
	return (MSHELL_SUCCESS);
}

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
char	*logic_input_right(char *input, const char *operator, t_body *msh)
{
	int		i;
	char	*right_input;

	i = -1;
	right_input = NULL;
	input = ft_strrstr_ip(input, operator, ft_strlen(input));
	if (!input)
		return (NULL);
	right_input = ft_substr(input, 2, ft_strlen(&input[2]));
	if (!right_input)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	return (right_input);
}

/**
 * @brief	Extracts the portion of input to the left of a logical operator.
 *
 *			Finds the last occurrence of the operator ("&&" or "||") and
 *			returns a newly allocated substring containing the commands
 *			to the left of it.
 *
 * @param	input		Input command string.
 * @param	operator	Logical operator to split at.
 * @param	msh			Shell context, used for error handling on malloc.
 *
 * @return	Newly allocated string with left-side commands.
 */
char	*logic_input_left(char *input, const char *operator, t_body *msh)
{
	int		i;
	char	*tmp;
	char	*left_input;

	i = 0;
	left_input = NULL;
	tmp = ft_strrstr_ip(input, operator, ft_strlen(input));
	if (!tmp)
		return (NULL);
	i = tmp - input;
	left_input = ft_substr(input, 0, i);
	if (!left_input)
		shell_forcend(MSHELL_FAILURE, "malloc", msh);
	return (left_input);
}
