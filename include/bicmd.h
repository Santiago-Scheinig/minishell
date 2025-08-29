/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/29 19:45:02 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BICMD_H
# define BICMD_H
# include <unistd.h>
# include <limits.h>
# include "minishell.h"

void	free_env_list(t_var *env);

t_var	*b_export(char **envp, t_var *head, char **args);

t_var	*add_env(t_var *head, char *new_env);

char	*shell_getenv(t_list *lst_var, const char *name);

void	print_export(t_list *env_lst);

void	print_env(char **envp);

void	echo(char **args);

int		cd(t_body *minishell, char **args);

int		pwd(char **args);

void	built_end(char *name, char *type, char *flags, char error);

#endif
