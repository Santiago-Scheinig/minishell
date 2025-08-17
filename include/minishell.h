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
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <signal.h>
# include <stdio.h>
# include "libft.h"

extern volatile sig_atomic_t	g_signal_received;

typedef struct s_env
{
	struct	s_env *next;
	char	*value;
	char	*name;
	int		exported;
}	t_env;

typedef struct s_cmd
{
	int		built_in;
	int		infile;
	int		outfile;
	char	*pathname;
	char	**argv;
}	t_cmd;

typedef struct s_body
{
	struct termios	term;
	t_cmd			**commands;
	t_env			*env;
	char			*prompt;
	char			*input;
	int				*pipe_child;
	int				pipe_child_count;
	int				errno;
}	t_body;

t_env	*add_env(t_env *head, char *new_env);

t_env	*init_envp(char **envp);

void	recive_signals(t_body *minishell);

void	cleanup(t_body *minishell);

void	initialization(void);

int		path_minishell(t_body *minishell);

#endif
