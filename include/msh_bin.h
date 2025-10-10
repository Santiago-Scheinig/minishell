/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_bin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/10 09:17:47 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_BIN_H
# define MSH_BIN_H

# include "shell_std.h"

/**
 * Builterr
 */
int			bin_cd(char **args, t_list **envp);

/**
 * Builterr
 */
int			bin_echo(char **args);

/**
 * Builterr
 */
int			bin_env(char **args, char **envp);

/**
 * Builterr
 */
int			bin_exit(char **args, t_body *msh);

/**
 * Builterr
 */
int			bin_export(char **argv, char ***envp, t_list *lst_envp);

/**
 * Builterr
 */
int			bin_import(char ***argv, char ***envp, t_list *lst_envp);

/**
 * Builterr
 */
int			bin_pwd(char **args, t_list *lst_t_var);

/**
 * Need fix
 */
int			bin_unset(char **arg, char ***envp, t_list **head_t_var);



/**
 * Builterr
 */
int			print_lst_t_var(t_list *lst_t_var);

/**
 * Builterr
 */
int			is_valid_var(char *arg, int print);

/**
 * Need docs
 */
char		**ft_remove_arr(char **arr, int index);

#endif
