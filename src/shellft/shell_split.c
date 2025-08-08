/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:45:24 by sscheini          #+#    #+#             */
/*   Updated: 2025/08/08 18:48:23 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shellft.h"

/**
 * Initializes the ARRAY of STRINGS dividing WORDS and OPERATORS into 
 * individual STRINGS.
 * @param wrdstr An pointer to an empty initialized ARRAY of STRINGS.
 * @param s	The main string to split.
 * @param word_amount The max amount of splitted words.
 * @return In success, returns the pointer to wrdstr. NULL in case of memory
 * allocation error.
 * @note Operators such as '\\' and ';' aren't interpreted by minishell, and as
 * such they are ignored as independent words. 
 */
static char	***create_split(char ***wrdstr, const char *s, int word_amount)
{
	t_token_type	type;
	int				token_len;
	int				i;

	i = -1;
	while (++i < word_amount)
	{
		type = get_token_type((char *)&s[0]);
		if (!type)
			token_len = word_len((char *) s);
		else
			token_len = operator_len(type);
		(*wrdstr)[i] = ft_substr(s, 0, token_len);
		if (!((*wrdstr)[i]))
			return (memfree((*wrdstr), i));
		while(token_len)
		{
			token_len--;
			s++;
		}
		while((*s) == ' ' || (*s) == '\\' || (*s) == ';')
			s++;
	}
	(*wrdstr)[i] = (void *)(0);
	return (wrdstr);
}

/**
 * Counts the amount of words found on a STRING; a word being all the 
 * characters between spaces, shell operators and/or '\0'.
 * @param s The STRING where to count words.
 * @return An INT with the amount of words counted.
 * @note This function will ignore any divisor character coincidences that 
 * happen to be inside of single and double quotes 
 * (as long they open and close).
 */
static int	word_count(const char *s)
{
	const char	*tmp;
	int			i;
	int			type;
	int			count;

	i = 0;
	count = 0;
	if (s[0] == 0)
		return (0);
	type = get_token_type((char *) &s[0]);
	if (!type && s[0] != ' ')
		count++;
	tmp = shell_word_strchr(&s[0]);
	type = get_token_type((char *) &tmp[0]);
	if (type == HEREDOC || type == REDIR_APPEND)
		i++;
	if (tmp != NULL && tmp[i + 1])
		count += word_count(&tmp[i + 1]);
	return (count);
}

/**
 * Counts the amount of shell operators found on a STRING; a shell operator
 * being "|", "<<", ">>", "<" and ">".
 * @param s The STRING where to count the shell operators.
 * @return An INT with the amount of words counted.
 * @note This function will ignore any C coincidences that happen to be inside
 * of single and double quotes (as long they open and close).
 */
static int	operator_count(const char *s)
{
	const char	*tmp;
	int			i;
	int			type;
	int			count;
	
	i = 0;
	count = 0;
	if (s[i] == 0)
		return (0);
	type = get_token_type((char *) &s[i]);
	if (type > WORD && type < END)
		count++;
	if (type && type != END)
		i++;
	if (type == REDIR_APPEND || type == HEREDOC)
		i++;
	tmp = shell_operator_strchr(&s[i]);
	if (tmp != NULL)
		count += operator_count(&tmp[0]);
	return (count);
}

/**
 * Splits a STRING into an ARRAY of STRINGS, divided between words and 
 * shell operators, while also using spaces as divisor characters.
 * @param s The STRING to split.
 * @return An ARRAY of STRINGS that includes every divided shell token and is 
 * NULL terminated.
 * @note This function will take any amount of characters that happen to be 
 * inside of single and double quotes (as long they open and close) as 
 * WORD tokens. Additionally, if quotes aren't properly closed, they 
 * are treated as unexistent.
 */
char	**shell_split(const char *s)
{
	char	**wrdstr;
	int		word_amount;
	int		i;

	if (!s)
		return (NULL);
	if (s[0] == 0)
		return (ft_calloc(1, sizeof(char *)));
	i = 0;
	word_amount = operator_count(s);
	word_amount += word_count(s);
	wrdstr = (char **) malloc((word_amount + 1) * sizeof(char *));
	if (!wrdstr)
		return (NULL);
	if (create_split(&wrdstr, s, word_amount) == NULL)
		return (NULL);
	return (wrdstr);
}
