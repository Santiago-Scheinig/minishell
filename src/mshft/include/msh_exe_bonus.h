/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_exe_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 14:29:32 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 14:52:28 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_EXE_BONUS_H
# define MSH_EXE_BONUS_H

# include "shell_std.h"

int	logic_execution(char *input, t_body *msh);

int	logic_subshell(char	*input, t_body *msh);

#endif