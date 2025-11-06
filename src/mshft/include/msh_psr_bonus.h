/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_psr_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 18:06:09 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/06 14:29:39 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MSH_PSR_BONUS_H
# define MSH_PSR_BONUS_H

# ifndef OPERATORS
#  define OPERATORS "\'\"\\;|<>*"
# endif

# include "shell_std.h"
# include <dirent.h>

typedef enum e_logic_type
{
	LPARENTHESIS,
	RPARENTHESIS,
	AND,
	OR,
	INPUT,
	UNSET,
}	t_logic_type;

typedef struct s_logic
{
	char				*str;
	int					type;
}	t_logic;

t_logic	*shell_newlogic(char *str);

void	shell_dellogic(void *content);

int	logic_parser(char **logic_input, t_body *msh);

int	parser_wildcard(t_body *msh);

int	copy_name(char ***names, char *dir);

int	wildcard(char **names, char ***matches, t_list *head);

#endif