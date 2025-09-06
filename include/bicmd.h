/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/06 18:40:36 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BICMD_H
# define BICMD_H
# include "minishell.h"
# include <unistd.h>
# include <limits.h>

void	free_env_list(t_var *env);

char	**export_no_equal(char **args, char **envp);

char	**export_no_dup(char **args);

t_list	*b_export(char ***envp, t_list *head, char **args);

t_var	*add_env(t_var *head, char *new_env);

int		set_equal(t_var *aux, char **envp, char *sign, char *new_env);

char	*shell_getenv(t_list *lst_var, const char *name);

void	print_export(t_list *env_lst);

void	print_env(char **envp);

char	**shell_realloc(char **args, char **envp);

void	echo(char **args);

int		cd(char **args, t_list *envp);

int		pwd(char **args);

char	**unset(char **envp, t_list *env_lst, char **name);

t_var	*create_envp(const char *envp);

int	built_end(char *name, char *type, char *flags, char error);

char	*built_in(t_body *minishell, char *pathname, char **args, t_list *lst);

char	**ft_remove_arr(char **arr, int index);

#endif
