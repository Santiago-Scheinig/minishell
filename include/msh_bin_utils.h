/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_bin_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 19:51:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/09 06:05:54 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_BIN_UTILS
# define MSH_BIN_UTILS

# include "msh_bin.h"

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