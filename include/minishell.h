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
# include <stdio.h>
# include <signal.h>

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