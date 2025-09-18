/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execmd.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:35:34 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/18 14:49:06 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECMD_H
# define EXECMD_H
# include <sys/wait.h>
# include <unistd.h>
# include <fcntl.h>
# include "libft.h"

typedef struct s_files
{
	int	oldin;
	int	oldout;
	int	exein;
	int	exeout;
}	t_files;

typedef struct s_cmd
{
	int		built_in;
	int		heredoc[2];
	char	*limitator;
	char	*pathname;
	char	**argv;
	t_files		fd;
}	t_cmd;

// void	exe_getpath()
// void exe_verify()
// void	fd_setexe()
// void	fd_endexe()
// void	fd_setold()
// void	fd_endold()
// void	fd_heredoc()

#endif
