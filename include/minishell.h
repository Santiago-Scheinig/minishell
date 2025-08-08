/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 15:51:55 by ischeini          #+#    #+#             */
/*   Updated: 2025/07/24 15:51:55 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <stdio.h>

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_cmd
{
	int		built_in;
	int		heredoc[2];
	int		infile;
	int		outfile;
	char	*limitator;
	char	*pathname;
	char	**argv;
}	t_cmd;

/*struct termios
{
	tcflag_t	c_cflag;   // Control flags
	tcflag_t	c_lflag;   // Locals flags
	tcflag_t	c_iflag;   // Enter flags
	tcflag_t	c_oflag;   // Out flags
	cc_t		c_cc[NCCS];// Special caracters (como EOF, INTR, etc)
};*/

typedef struct s_body
{
	struct termios	term;
	t_cmd			**commands;
	char			*input;
	int				*pipe_child;
	int				pipe_child_count;
	int				errno;
}	t_body;

void	recive_signals(t_body *minishell);

//temporaly cleanup for test
void	cleanup(t_body *minishell);

#endif
typedef struct s_body
{
	t_list	*cmd_lst;//sigend parser
	t_list	*token_lst;//sigend parser
	char	*input;//always freed, even before error
	int		*pipe_child;
	int		pipe_child_count;
	int		errno;
}	t_body;

void	parser(t_body *minishell, char *input);

void	recive_signals(t_body *minishell);

//temporaly cleanup for test
void	cleanup(t_body *minishell);

#endif