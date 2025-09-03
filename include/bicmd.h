/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bicmd.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 18:15:02 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/03 21:46:03 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BICMD_H
# define BICMD_H
# include <unistd.h>
# include <limits.h>

/*Id like to change this to a envp structure, inside a t_list structure.*/
/*If the only functions that are able to edit this, are shell functions,*/
/*Then we put this inside of shellft.h instead, more clean.				*/
typedef struct s_var
{
	char				*value;
	char				*name;
	int					exported;
}	t_var;

void	free_env_list(t_var *env);

t_list	*b_export(char **envp, t_list *head, char **args);

t_var	*add_env(t_var *head, char *new_env);

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

char	**ft_remove_arr(char **arr, size_t index);

#endif
