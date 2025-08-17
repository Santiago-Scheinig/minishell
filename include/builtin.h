/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/17 19:51:34 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "minishell.h"
# include <unistd.h>
# include <limits.h>
# include <stdio.h>

t_env	*built_export(t_body *minishell, char **args, char **envp);

t_env	*add_env(t_env *head, char *new_env);

t_env	*init_envp(char **envp);

char	*shell_getenv(t_body *minishell, const char *name);

void	print_export(t_env *env_lst);

void	print_env(t_env *env_lst);

void	built_echo(char **args);

void	sort_env(t_env *head);

int		built_cd(t_body *minishell, char **args);

int		built_pwd(char **args);

#endif
