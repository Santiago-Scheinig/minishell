/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/09 20:55:44 by sscheini          #+#    #+#             */
/*   Updated: 2025/10/10 06:01:38 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "libft_utils.h"

/**
 * @brief	Ends variable argument processing and frees t_print members.
 *
 *			Used internally to cleanup va_list and dynamically allocated
 *			strings or flags in t_print. Returns -1 if va_list is valid,
 *			0 otherwise.
 *
 * @param	args	va_list to end (may be NULL).
 * @param	text	t_print struct containing flags and str to free.
 *
 * @return	-1 if va_list ended, 0 if args was NULL.
 */
static	int	forcend(va_list args, t_print text)
{
	int	ans;

	ans = -1;
	if (args)
		va_end(args);
	else
		ans = 0;
	if (text.flags)
		free(text.flags);
	if (text.str)
		free(text.str);
	return (ans);
}

/**
 * @brief	Retrieves the next argument as a string based on specifier.
 *
 *			Handles conversion for %c, %s, %d, %i, %u, %x, %X, %p, and %%.
 *			Allocates memory for the returned string. Handles NULL strings
 *			by returning "(null)".
 *
 * @param	spf		Conversion specifier character.
 * @param	args	Variable argument list to extract value from.
 *
 * @return	Pointer to dynamically allocated string representing argument.
 */
static	char	*getarg(char spf, va_list args)
{
	void	*str;

	str = NULL;
	if (spf == '%')
		str = ft_chrdup('%');
	else if (spf == 'c')
		str = ft_chrdup(va_arg(args, int));
	else if (spf == 'u')
		str = ft_utoa_base(va_arg(args, unsigned int), "0123456789");
	else if (spf == 'd' || spf == 'i')
		str = ft_itoa(va_arg(args, int));
	else if (spf == 'x' || spf == 'X')
		str = ft_utoa_base(va_arg(args, unsigned int), ft_sethexa(spf));
	else if (spf == 'p')
	{
		str = va_arg(args, void *);
		if (!str)
			return (ft_strdup("(nil)"));
		str = ft_ptrdup(ft_utoa_base((size_t) str, ft_sethexa(spf)), 'p');
	}
	else if (spf == 's')
		str = ft_strdup(va_arg(args, char *));
	if ((!str && spf == 's'))
		return (ft_strdup("(null)"));
	return ((char *) str);
}

/**
 * @brief	Applies flags, width, and precision to a string argument.
 *
 *			Handles strings, characters, percent signs, hexadecimal, and
 *			decimal formatting. Delegates to txtwth(), hexwth(), or
 *			dgtwth() depending on type.
 *
 * @param	str		String to format.
 * @param	flags	Array of printf-like flags.
 * @param	str_len	Length of the string before formatting.
 *
 * @return	Pointer to formatted string (malloced), or NULL on error.
 */
static	char	*flagstr(char *str, int *flags, int str_len)
{
	if (flags[7] == 's' && flags[6] <= 5 && !ft_strncmp(str, "(null)", 6))
		if (flags[6] != -1)
			flags[6] = 0;
	if (flags[7] == 's' || flags[7] == 'c' || flags[7] == '%')
	{
		if (flags[7] == 's' && flags[6] < str_len && flags[6] != -1)
			str = txtwth(flags, str, flags[6]);
		else
			str = txtwth(flags, str, str_len);
	}
	else if (flags[7] == 'x' || flags[7] == 'X' || flags[7] == 'p')
	{
		if (!ft_strncmp("0", str, 2) && flags[6] == 0)
			str = txtwth(flags, str, 0);
		else
			str = hexwth(flags, str, str_len);
	}
	else
		str = dgtwth(flags, str, str_len);
	return (str);
}

/**
 * @brief	Processes and prints a formatted t_print struct.
 *
 *			Handles special cases for null characters in %c. Applies
 *			flagstr() for non-character conversions. Updates the printed
 *			characters count. Cleans up memory before returning.
 *
 * @param	text	t_print struct containing flags and string to print.
 * @param	count	Pointer to total printed character count.
 *
 * @return	Total characters printed, or -1 on error.
 */
static	int	printv(t_print text, int *count, int fd)
{
	int		zero;

	zero = 1;
	if (text.flags[7] == 'c' && !text.str[0])
	{
		zero = 0;
		if (text.flags[5] > 1)
		{
			free(text.str);
			text.str = ft_calloc(text.flags[5] - 1, sizeof(char *));
			if (!text.str)
				return (-1);
			ft_memset(text.str, ' ', text.flags[5] - 1);
		}
	}
	else
		text.str = flagstr(text.str, text.flags, ft_strlen(text.str));
	if (!text.str)
		return (-1);
	if (!zero && text.flags[0])
		*count += ft_putchar_fd(zero++, fd);
	*count += ft_putstr_fd(text.str, fd);
	if (!zero)
		*count += ft_putchar_fd(zero, fd);
	return (forcend(NULL, text));
}

/**
 * @brief	Formats and writes output to a given file descriptor.
 *
 *			Works similarly to ft_printf, but writes the formatted
 *			output to the file descriptor 'fd'. Supports format
 *			specifiers like %d, %i, %u, %x, %X, %c, %s, %p, and %%.
 *			Uses helper functions to parse flags, width, precision,
 *			and apply formatting before writing each segment.
 *
 * @param	fd	File descriptor to write the formatted output to.
 * @param	s	Format string containing literal text and specifiers.
 * @param	...	Additional arguments corresponding to format specifiers.
 *
 * @return	Number of characters written on success, or -1 on error.
 */
int	ft_fprintf(int fd, char const *s, ...)
{
	va_list	args;
	t_print	text;
	int		count;

	count = 0;
	va_start(args, s);
	while (*s)
	{
		if (*s == '%')
		{
			text.flags = flagdescriptor(s + 1);
			if (!text.flags)
				return (forcend(args, text));
			text.str = getarg(text.flags[7], args);
			if (!text.str)
				return (forcend(args, text));
			s = ft_strchr(s + 1, text.flags[7]) + 1;
			if (printv(text, &count, fd) < 0)
				return (forcend(args, text));
		}
		else
			count += ft_putchar_fd(*s++, fd);
	}
	va_end(args);
	return (count);
}
