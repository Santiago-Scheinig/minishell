/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_tsh.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:16:00 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 17:35:52 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_TSH_H
# define MSH_TSH_H

# include <signal.h>
# include <errno.h>

extern volatile sig_atomic_t	g_signal_received;

int	siginit(void);

/**
 * 
 * 
 */

typedef enum e_error
{
	MSHELL_FATAL = -1,
	MSHELL_SUCCESS,
	MSHELL_FAILURE,
	MSHELL_MISSUSE,
	MSHELL_CMD_INVEXE = 126,
	MSHELL_CMD_NOTEXE,
	MSHELL_SIG_HANDLR,
}	t_error;

void	cleanup(t_body *minishell);

int		forcend(t_body *minishell, char *function, t_error number);

int		redirend(char *argv, t_error number);

int		sigend(const char *next, t_error number, t_body *minishell);

void	end_minishell(t_body *minishell);

#endif