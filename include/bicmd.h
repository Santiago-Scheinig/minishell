/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/17 17:43:17 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BICMD_H
# define BICMD_H
# include "minishell.h"
# include <unistd.h>
# include <limits.h>

void	free_env_list(t_var *env);

char	**export_no_equal(char **args, t_list *lst);

char	**export_no_dup(char **args);

t_list	*b_export(char ***envp, t_list *head, char **args);

t_var	*add_env(t_var *head, char *new_env);

int		set_equal(t_var *aux, char **envp, char *sign, char *new_env);

int		env(char **args, char **envp, t_list *env_lst);

void	print_export(t_list *env_lst);

int		b_exit(char **args, t_body *minishell);

void	print_env(char **envp, t_list *env_lst);

char	**shell_realloc(char **args, char **envp, size_t size);

void	echo(char **args);

int		cd(char **args, t_list *envp);

void	end_minishell(t_body *minishell);

int		pwd(char **args);

char	**unset(char **envp, t_list *env_lst, char **name);

int		built_end(char *name, char *type, char *flags, char error);

char	*built_in(t_body *minishell, char *pathname, char **args, t_list *lst);

char	**ft_remove_arr(char **arr, int index);

int		is_valid_identifier(char *arg);

char	**ft_isal_num(char **args, t_list *head);

int		change_value_env(t_var *aux, char ***envp, char *new_env, int export);

t_list	*new_envp(char **new_env, t_list *head, int export);

t_list	*inport(char ***envp, t_list *head, char **args);

#endif
