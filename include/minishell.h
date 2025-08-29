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
	struct s_env	*current_next;
	char			*value;
	char			*name;
	int				exported;
}	t_env;

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


/**
 * Termios structure detailed
 * 
 * @param c_cflag	A TCFLAG_T sructure to control all flags.
 * @param c_lflag	A TCFLAG_T structure to control local flags.
 * @param c_iflag	A TCFLAG_T structure to enter flags.
 * @param c_oflag	A TCFLAG_T structure to remove flags.
 * @param c_cc		A CC_T structure with size [NCCS] for special characters
 */

/**
 * Struct used to save the enviroment variables of the minishell.
 * 
 * @param term			Termios structure linked to the terminal initialization
 * @param commands
 * @param input
 * @param childs_pid
 * @param errno
 */
typedef struct s_body
{
	struct termios	term;
	t_list			*cmd_lst;
	t_list			*token_lst;
	t_list			*dup_envp_lst;
	char			*prompt;
	char			*input;
	char			**input_split;
	char			**dup_envp;
	int				*childs_pid;
	int				errno;
}	t_body;

t_body	*init_env(t_body *minishell, char **envp);

t_env	*create_envp(char *envp, int *error);

t_env	*add_env(t_env *head, char *new_env);

t_env	*init_envp(char **envp);

char	**ft_realloc(char **envp, char **new_envp, int size);

char	*built_in(t_body *minishell, char *pathname, char **args, t_env *envp);

//temporaly cleanup for test
void	print_export(t_env *env_lst);

void	cleanup(t_body *minishell);

void	print_env(t_env *env_lst);

void	parser(t_body *minishell);

void	initialization(void);

int		shell_prompt(t_body *minishell);

#endif
