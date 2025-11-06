/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:06:09 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/05 16:06:45 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_BONUS_H
# define MSH_PSR_BONUS_H

# ifndef OPERATORS
#  define OPERATORS "\'\"\\;|<>*"
# endif

# include "shell_std.h"
# include <dirent.h>

int	parser_wildcard(t_body *msh);

int	copy_name(char ***names, char *dir);

int	wildcard(char **names, char ***matches, t_list *head);

#endif