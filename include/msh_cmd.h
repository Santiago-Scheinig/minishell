/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_cmd.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ischeini <ischeini@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/24 13:38:40 by ischeini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_CMD_H
# define MSH_CMD_H

# include "lib/msh_std.h"
# include "lib/msh_tsh.h"

void	free_env_list(t_var *env);

char	**export_no_equal(char **args, char ***envp, t_list *lst);

char	**export_no_dup(char **args);

int		msh_export(char ***envp, t_list **head, char **args);

t_var	*add_env(t_var *head, char *new_env);

int		set_equal(t_var *aux, char **envp, char *sign, char *new_env);

int		msh_env(char **args, char **envp);

void	print_export(t_list *env_lst);

void	msh_exit(char **args, t_body *minishell);

void	print_env(char **envp);

int		exp_resize(char **args, char ***envp);

void	msh_echo(char **args);

int		msh_cd(char **args, t_list *envp);

void	end_minishell(t_body *minishell);

int		msh_pwd(char **args);

int		msh_unset(char ***envp, t_list **env_lst, char **name);

int		built_end(char *name, char *type, char *flags, char error);

int		built_in(char **args, t_body *minishell);

char	**ft_remove_arr(char **arr, int index);

int		is_valid_identifier(char *arg);

char	**ft_isal_num(char **args, t_list *head, char ***envp);

int		msh_import(char ***envp, t_list **head, char **args);

int		change_value_env(t_var *aux, char ***envp, char **new_env, int export);

int		new_envp(char **new_env, t_list *head, int export);

#endif
