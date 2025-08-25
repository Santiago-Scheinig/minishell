/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/08/25 19:39:48 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BICMD_H
# define BICMD_H
# include <unistd.h>
# include <limits.h>
# include "minishell.h"

void	free_env_list(t_env *env);

t_env	*built_export(t_body *minishell, char **args, char **envp);

t_env	*add_env(t_env *head, char *new_env);

char	*shell_getenv(t_body *minishell, const char *name);

void	print_export(t_env *env_lst);

void	print_env(t_env *env_lst);

void	built_echo(char **args);

void	sortenv(t_env *head);

int		built_cd(t_body *minishell, char **args);

int		built_pwd(char **args);

#endif
