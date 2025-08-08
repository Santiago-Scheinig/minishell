/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_enviroment_var_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:58:42 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/08 18:31:30 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "parser.h"

int	envar_pathstart(char *envp)
{
	int i;

	i = 1;
	while (envp[i] != '=')
		i++;
	return (i);
}

char	*envar_pathval(char *envp)
{
	
}

char	*envar_pathsrc(char *env_var)
{
	char	*new_path;
	int		env_var_len;
	int		i;

	i = -1;
	env_var_len = envlen(env_var);
	new_path = ft_calloc((env_var_len + 2), sizeof(char));
	if (!new_path)
		return (NULL);
	while(++i < env_var_len)
		new_path[i] = env_var[i];
	env_var[i] = '=';
	return (new_path);
}

int	envar_len(char *env_var)
{
	int	i;

	i = 1;
	while (ft_isalnum(env_var[i]) || env_var[i] == '_')
		i++;
	return (i);
}