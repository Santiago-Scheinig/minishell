/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:06:13 by sscheini          #+#    #+#             */
/*   Updated: 2025/09/23 19:00:58 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "msh_psr.h"

/**
 * COMMENT PENDING
 */
static int	mask_quoted(char *str, char *mask_str, char quote)
{
	int	i;

	i = 0;
	mask_str[i] = 'O';
	while (str[++i] && str[i] != quote)
	{
		if (quote == '\'')
			mask_str[i] = 'S';
		else if (quote == '\"')
			mask_str[i] = 'D';
	}
	mask_str[i] = 'O';
	return (i);
}

/**
 * COMMENT PENDING
 */
char	*mask_dup(char *str)
{
	int		i;
	char	quote;
	char	*mask_str;
	
	mask_str = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!mask_str)
		return (NULL);
	i = -1;
	while (str[++i])
	{
		quote = 0;
		if (str[i] == '\"' && str[i + 1] && ft_strchr(&str[i + 1], '\"'))
			quote = '\"';
		else if (str[i] == '\'' && str[i + 1] && ft_strchr(&str[i + 1], '\''))
			quote = '\'';
		if (quote)
			i += mask_quoted(&str[i], &mask_str[i], quote);
		else if (str[i] == ';' || str[i] == '\\'
			|| str[i] == '\'' || str[i] == '\"')
			mask_str[i] = 'O';
		else
			mask_str[i] = 'N';
	}
	return (mask_str);
}

static int	hdoc_mask(char *limit_mask, char **line, t_list *envp, t_pair aux)
{
	int	i;
	int	exp;

	i = -1;
	exp = 1;
	while(limit_mask[++i])
		if (limit_mask[i] == 'S' || limit_mask[i] == 'D')
			exp = 0;
	if (exp)
		if (envar_syntax(line, NULL, envp, aux.value))
			return (MSHELL_FAILURE);
	if (write(aux.var, (*line), ft_strlen((*line))) == -1)
		return(MSHELL_FAILURE);
	return (MSHELL_SUCCESS);
}

static int	hdoc_end(char *limitator, int line, int heredoc[2])
{
	const char *msg[3] = {
		"msh: warning:",
		"here-document at line",
		"delimited by end-of-file",
	};

	if (errno == ENOMEM)//forcend should check on errno instead of following msg to know what to print?
	{
		close(heredoc[0]);
		close(heredoc[1]);
		return (-1);//i also need to verify if read failed and print a message accordingly
	}
	ft_printfd(2, "\n%s %s %i %s ", msg[0], msg[1], line, msg[2]);
	ft_printfd(2, "(wanted '%s')\n", limitator);
	close(heredoc[1]);
	return (heredoc[0]);
}

/**
 * Reads from the STDIN until the specified LIMITATOR is written next to a
 * line jump '\n', writing everything that is sent into heredoc[1].
 * 
 * @param limitator The string that will work as LIMITATOR.
 * @param heredoc An array of INT which saves an already initialized pipe()
 * @return Returns heredoc[0] from where to read everything that was 
 * written on heredoc[1];
 * @note If the reading is interrupted before the LIMITATOR, the information
 * written on heredoc[0] will be sent to the next cmd and an error msg is printed 
 * on STDERR specifying the interruption issue.
 */
//if i set errno to 0 at the begging, then the verification is only if errno exists?
int	heredoc_dup(t_token *limit, int heredoc[2], t_list *envp, int exit_no)
{
	int		i;
	t_pair	aux;
	char	*line;

	i = 0;
	aux.var = heredoc[1];
	aux.value = exit_no;
	shell_memmove(limit->str, limit->str, limit->mask, ft_strlen(limit->str)+ 1);
	while (1)
	{
		i++;
		write(1, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			return (hdoc_end(limit->str, i, heredoc));
		if (line && !ft_strncmp(line, limit->str, ft_strlen(line) - 1))
		{
			free(line);
			break;
		}
		if (hdoc_mask(limit->mask, &line, envp, aux))
			return (hdoc_end(limit->str, i, heredoc));
		free(line);
	}
	close(heredoc[1]);
	return (heredoc[0]);
}
