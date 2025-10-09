/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_bin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/10/09 06:06:25 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_CMD_H
# define MSH_CMD_H

# include "shell_std.h"

/**
 * Builterr
 */
int			msh_cd(char **args, t_list **envp);

/**
 * Builterr
 */
int			msh_echo(char **args);

/**
 * Builterr
 */
int			msh_env(char **args, char **envp);

/**
 * Builterr
 */
int			msh_exit(char **args, t_body *msh);

/**
 * Builterr
 */
int			msh_export(char **argv, char ***envp, t_list *lst_envp);

/**
 * Builterr
 */
int			msh_import(char ***argv, char ***envp, t_list *lst_envp);

/**
 * Builterr
 */
int			msh_pwd(char **args, t_list *lst_t_var);

/**
 * Need fix
 */
int			msh_unset(char **arg, char ***envp, t_list **head_t_var);


#endif
