/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:05:54 by ischeini          #+#    #+#             */
/*   Updated: 2025/09/29 15:36:03 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_cmd.h"

/**
 * Validates that the provided identifier is suitable for export.
 * 
 * @param arg Candidate string like "NAME" or "NAME=VALUE".
 * 
 * Ensures first char is alphabetic or '_' and subsequent chars are
 * alphanumeric or '_'.
 * Prints an error via built_end on invalid identifiers.
 * 
 * @return 0 if valid, non-zero error code otherwise.
 */
int	is_valid_identifier(char *arg)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (built_end("export", "Not valid identifier", arg, '\0'));
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (built_end("export", "Not valid identifier", arg, '\0'));
		i++;
	}
	return (0);
}

/**
 * Processes and validates export arguments; returns normalized args.
 * 
 * @param args Array of argument strings to export.
 * @param head Linked list head of current variables.
 * @param envp Pointer to environment array pointer (used by helpers).
 * 
 * Returns a possibly modified args array (duplicates removed, errors reported)
 * If no args provided, prints export list and returns NULL.
 * 
 * @return Adjusted args array or NULL.
 */
char	**ft_isal_num(char **args, t_list *head, char ***envp)
{
	int	i;
	int	j;

	i = 0;
	if (!args[0])
	{
		print_export(head);
		return (NULL);
	}
	if (args[0][0] == '-')
	{
		built_end("export", "Invalid flags", "[name[=value] ...]", args[0][1]);
		return (NULL);
	}
	j = -1;
	while (args[++j])
	{
		if (is_valid_identifier(args[j]) && args[j][i] != '_')
			args = ft_remove_arr(&args[0], j);
	}
	args = export_no_dup(args);
	args = export_no_equal(args, envp, head);
	return (args);
}

/**
 * Attempts to change the stored value of an existing variable.
 * 
 * @param aux Pointer to the t_var node representing the variable.
 * @param envp Pointer-to-pointer to the envp array used for string updates.
 * @param new_env Array with new_env[0] equal to the new "NAME=VALUE" string.
 * @param export Flag indicating export behavior.
 * 
 * Matches names and updates export flag or value when appropriate.
 * 
 * @return 0 on handled change, 1 if sizes differ, or non-zero on error.
 */
int	change_value_env(t_var *aux, char ***envp, char **new_env, int export)
{
	size_t	i;
	size_t	j;
	char	*sign;
	int		nbr;

	i = ft_strlen(aux->name);
	j = 0;
	nbr = 0;
	while (new_env[0][j] && new_env[0][j] != '=')
		j++;
	if (j != i)
		return (1);
	if (export == 1 && aux->exported == 0)
	{
		aux->exported = export;
		exp_resize(new_env, envp);
		return (0);
	}
	sign = ft_strchr(new_env[0], '=');
	nbr = set_equal(aux, envp[0], sign, new_env[0]);
	return (nbr);
}

/**
 * Appends new variables (not previously in list) to the variable list.
 * 
 * @param new_env Array of strings "NAME" or "NAME=VALUE" for new entries.
 * @param head Linked list head where new nodes will be appended.
 * @param export Export flag to set on created nodes.
 * 
 * Allocates nodes and appends them to the list tail.
 * 
 * @return 0 on success, non-zero on failure.
 */
int	new_envp(char **new_env, t_list **head, int export)
{
	t_list	*node;
	t_var	*content;
	int		i;

	i = -1;
	while (new_env[++i])
	{
		content = create_envp(new_env[i], export);
		if (!content)
			return (built_end("export", "System failed", NULL, '\0'));
		node = ft_lstnew(content);
		if (!node)
			return (built_end("export", "System failed", NULL, '\0'));
		ft_lstadd_back(head, node);
	}
	return (0);
}

/**
 * Built-in 'export' command handler.
 * 
 * @param envp Pointer-to-pointer to environment array.
 * @param head Pointer-to-list head of current variables.
 * @param args Arguments for export command.
 * 
 * Validates, updates existing variables, resizes envp and appends new vars.
 * 
 * @return 0 on success, non-zero on failure.
 */
int	msh_export(char ***envp, t_list **head, char **args)
{
	t_list	*tmp;
	t_var	*aux;
	int		i;
	int		j;

	tmp = *head;
	j = 0;
	args = ft_isal_num(args, *head, envp);
	if (!args)
		return (1);
	while (tmp && tmp->content)
	{
		i = -1;
		aux = (t_var *)tmp->content;
		while (args[++i])
			if (!ft_strncmp(aux->name, args[i], ft_strlen(aux->name)))
				if (!change_value_env(aux, envp, &args[i], 1))
					args = ft_remove_arr(&args[0], i);
		tmp = tmp->next;
	}
	if (exp_resize(args, envp) || new_envp(args, head, 1))
		return (1);
	return (0);
}
