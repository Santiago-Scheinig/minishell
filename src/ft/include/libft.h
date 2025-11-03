/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sscheini <sscheini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 17:37:54 by sscheini          #+#    #+#             */
/*   Updated: 2025/11/03 17:13:42 by sscheini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

/*--------------------------------------------------------------------------*/
/*----------------------------------MACROS----------------------------------*/
/*--------------------------------------------------------------------------*/

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

/*--------------------------------------------------------------------------*/
/*--------------------------------INCLUSIONS--------------------------------*/
/*--------------------------------------------------------------------------*/

# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <strings.h>
# include <limits.h>
# include <unistd.h>

/*--------------------------------------------------------------------------*/
/*-----------------------------LIBFT STRUCTURES-----------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Node structure for a singly linked list.
 *
 *			Each node stores a generic pointer 'content' and a
 *			pointer to the next node in the list. Can be used
 *			for various data types by storing pointers in 'content'.
 */
typedef struct s_list
{
	void			*content;	//	Pointer to the data stored in the node.
	struct s_list	*next;		//	Pointer to next node in list, NULL if last
}	t_list;

/**
 * @brief	Represents a single formatted output conversion.
 *
 * 			Holds all necessary information for a single conversion
 * 			in printf-like functions, including formatting flags and
 * 			the resulting string after applying the conversion rules.
 *
 * @note	Used internally by ft_printf and ft_fprintf implementations.
 */
typedef struct s_print
{
	int		*flags;	// Pointer to array holding format flags
	char	*str;	// Formatted string corresponding to a single conversion
}	t_print;

/*--------------------------------------------------------------------------*/
/*-----------------------------------LIBFT----------------------------------*/
/*--------------------------------------------------------------------------*/

/**
 * @brief	Joins command-line arguments into a single string with a separator.
 *
 * 			Concatenates all strings from argv starting at index 1, inserting
 * 			the character 'c' between each argument. The resulting string 
 * 			always ends with the separator character. Memory is dynamically 
 * 			allocated for the result using ft_calloc().
 *
 * @param	argv	Array of argument strings, terminated by NULL.
 * @param	c		Character used as a separator between arguments.
 *
 * @note	The first element argv[0] is skipped (commonly the program name).
 * @note	The returned string ends with the separator and must be freed by
 *			the caller.
 *
 * @return	Newly allocated string containing the concatenated arguments with
 *			separators, or NULL if allocation fails.
 */
char		*ft_argjoin(char **argv, char c);

/**
 * @brief	Appends a string to a dynamically allocated array of strings.
 *
 * 			Expands the array 'arr' to hold one more string and adds 'str'
 *			at the end. The resulting array is NULL-terminated.
 *
 * @param	arr	Array of strings to append to.
 * @param	str	String to add to the array.
 *
 * @return	New array including 'str', or NULL on allocation failure.
 */
char		**ft_arradd_str(char **arr, char *str);

/**
 * @brief	Calculates the number of elements in a NULL-terminated array.
 *
 * 			Counts the number of non-NULL elements in the array pointed to
 * 			by arr, stopping when a NULL pointer is encountered. Returns 0
 * 			if arr is NULL or if the first element is NULL.
 *
 * @param	arr	Pointer to a NULL-terminated array of pointers.
 *
 * @note	This function works for any array of pointer types, such as
 *			strings or generic pointer lists.
 *
 * @return	Number of elements in the array before the terminating NULL.
 */
int			ft_arrlen(const void **arr);

/**
 * @brief	Converts a string representing a number in a custom base to int.
 *
 * 			Parses the string 'str' as a number in the base defined by the
 *			string 'base', and returns its integer value. Each character in
 *			'str' must belong to the provided base, which defines the symbol
 *			set and its positional value order.
 *
 *			Static helpers:
 *
 *				- ft_isbase():		Checks if a character exists within
 *									the base.
 *
 *				- ft_check_base():	Validates that 'str' contains only base
 *									characters and no invalid symbols.
 *
 *			The conversion multiplies the accumulated result by the base length
 * 			and adds the positional value of each character from left to right.
 *
 * @param	str		String to convert. Must only contain characters from base.
 * @param	base	String containing all valid symbols of the base.
 *
 * @note	If 'str' or 'base' contains invalid input, returns 0.
 * @note	Base length determines the radix (e.g., 2 for binary, 16 for hex).
 *
 * @return	Integer value represented by 'str' in the given base, or 0 on
 *			invalid input.
 */
int			ft_atoi_base(char *str, const char *base);

/**
 * @brief	Converts a string to an integer, handling optional whitespace 
 * 			and sign.
 *
 * 			Parses the string 'nptr' and returns its integer value. Leading
 *			whitespace characters are ignored, and an optional '+' or '-'
 *			sign is handled. Conversion stops at the first non-digit character.
 *
 *			Static helpers:
 *
 *				- ft_isspace():		Checks if a character is a whitespace.
 *
 *				- ft_issign():		Determines if a character is '+' or '-',
 *									returning its sign value.
 *
 * @param	nptr	String representing the number to convert.
 *
 * @note	If 'nptr' is NULL, the function returns 0.
 * @note	Conversion stops at the first non-digit character after optional
 *			whitespace and sign.
 *
 * @return	Integer value represented by the string, with sign applied.
 */
int			ft_atoi(const char *nptr);

/**
 * @brief	Converts a string to a long integer, handling optional whitespace
 *			and sign.
 *
 * 			Parses the string 'nptr' and returns its long integer value.
 *			Leading whitespace characters are ignored, and an optional '+'
 *			or '-' sign is handled. Conversion stops at the first non-digit
 *			character.
 *
 *			Static helpers:
 *
 *				- ft_isspace():		Checks if a character is a whitespace.
 *
 *				- ft_issign():		Determines if a character is '+' or '-',
 *									returning its sign value.
 *
 * @param	nptr	String representing the number to convert.
 *
 * @note	Conversion stops at the first non-digit character after optional
 *			whitespace and sign.
 *
 * @return	Long integer value represented by the string, with sign applied.
 */
long		ft_atol(const char *nptr);

/**
 * @brief	Sets a block of memory to zero.
 *
 * 			Fills the first 'n' bytes of the memory area pointed to by 's'
 *			with zeros. Does nothing if 'n' is zero.
 *
 * @param	s	Pointer to the memory area to zero.
 * @param	n	Number of bytes to set to zero.
 *
 * @note	This function is a wrapper around ft_memset with value 0.
 */
void		ft_bzero(void *s, size_t n);

/**
 * @brief	Allocates memory for an array and initializes it to zero.
 *
 * 			Allocates memory for 'nmemb' elements of 'size' bytes each.
 *			If allocation succeeds, the memory is set to zero using ft_bzero.
 *			Returns NULL on allocation failure or if the requested size is
 *			too large.
 *
 * @param	nmemb	Number of elements to allocate.
 * @param	size	Size of each element in bytes.
 *
 * @note	If either 'nmemb' or 'size' is zero, malloc(0) is called.
 * @note	This function prevents integer overflow in the multiplication.
 *
 * @return	Pointer to the newly allocated zero-initialized memory, or NULL.
 */
void		*ft_calloc(size_t nmemb, size_t size);

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
int			ft_fprintf(int fd, char const *s, ...);

/**
 * @brief	Reads the next line from a file descriptor.
 *
 *			Handles multiple file descriptors using a static array.
 *			Reads content incrementally using BUFFER_SIZE chunks.
 *			Returns the next line including the newline character if
 *			present.
 *
 * @param	fd		File descriptor to read from.
 *
 * @note	Supports multiple file descriptors simultaneously.
 * @note	Returns NULL on EOF or error.
 *
 * @return	Next line read from the file, or NULL if none.
 */
char		*ft_get_next_line(int fd);

/**
 * @brief	Checks if a character is alphanumeric.
 *
 * 			Determines if 'c' is either an alphabetic character or a digit.
 *
 * @param	c	Character to check.
 *
 * @return	true if 'c' is alphabetic or numeric, false otherwise.
 */
int			ft_isalnum(int c);

/**
 * @brief	Checks if a character is alphabetic.
 *
 * 			Determines if 'c' is a letter from 'A' to 'Z' or 'a' to 'z'.
 *
 *			Static helpers:
 *
 *				- isupper():	Checks if 'c' is an uppercase letter.
 *
 *				- islow():		Checks if 'c' is a lowercase letter.
 *
 * @param	c	Character to check.
 *
 * @return	1 if 'c' is alphabetic, 0 otherwise.
 */
int			ft_isalpha(int c);

/**
 * @brief	Checks if a character is an ASCII value.
 *
 * 			Determines if 'c' falls within the ASCII range 0 to 127.
 *
 * @param	c	Character to check.
 *
 * @return	1 if 'c' is an ASCII character, 0 otherwise.
 */
int			ft_isascii(int c);

/**
 * @brief	Checks if a character is a decimal digit.
 *
 * 			Determines if 'c' is a digit from '0' to '9'.
 *
 * @param	c	Character to check.
 *
 * @return	1 if 'c' is a digit, 0 otherwise.
 */
int			ft_isdigit(int c);

/**
 * @brief	Checks if a character is printable.
 *
 * 			Determines if 'c' is a printable ASCII character, including
 *			space.
 *
 * @param	c	Character to check.
 *
 * @return	1 if 'c' is printable, 0 otherwise.
 */
int			ft_isprint(int c);

/**
 * @brief	Converts an integer to a null-terminated string.
 *
 * 			Allocates memory and creates a string representing the integer
 *			'n'. Handles negative numbers and the special case of INT_MIN.
 *
 *			Static helpers:
 *
 *				- ft_swap():	Reverses the string, handling negative sign.
 *
 *				- ft_setstr():	Allocates the string and sets '-' if needed.
 *
 *				- ft_min():		Handles INT_MIN conversion safely.
 *
 *				- ft_nbrlen():	Calculates the number of digits in 'n'.
 *
 * @param	n	Integer to convert.
 *
 * @note	Memory is allocated for the returned string. Caller must free it.
 *
 * @return	Pointer to the newly allocated string representing 'n', or NULL
 *			on allocation failure.
 */
char		*ft_itoa(int n);

/**
 * @brief	Adds a new element at the end of a linked list.
 *
 * 			Appends the 'new' node at the end of the list pointed to by
 *			'lst'. If the list is empty, 'new' becomes the first element.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	new	Node to add at the end of the list.
 *
 * @note	Does nothing if 'new' is NULL.
 */
void		ft_lstadd_back(t_list **lst, t_list *new);

/**
 * @brief	Adds a new element at the beginning of a linked list.
 *
 * 			Inserts the 'new' node at the front of the list pointed to by
 *			'lst'. The previous first element becomes the second node.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	new	Node to add at the front of the list.
 *
 * @note	Does nothing if 'new' is NULL.
 */
void		ft_lstadd_front(t_list **lst, t_list *new);

/**
 * @brief	Clears a linked list and frees its memory.
 *
 * 			Iterates through the list pointed to by 'lst', applies the
 *			'del' function to free the content of each node, and frees
 *			each node. Sets '*lst' to NULL after clearing.
 *
 * @param	lst	Pointer to the pointer of the first node of the list.
 * @param	del	Function used to free the content of each node.
 *
 * @note	After this function, the list pointer will be NULL.
 */
void		ft_lstclear(t_list **lst, void (*del)(void *));

/**
 * @brief	Deletes a single node from a linked list.
 *
 * 			Applies the 'del' function to the node's content, then frees
 *			the node itself. Does not affect other nodes in the list.
 *
 * @param	lst	Node to delete.
 * @param	del	Function used to free the content of the node.
 *
 * @note	If 'lst->content' is NULL, only the node is freed.
 */
void		ft_lstdelone(t_list *lst, void (*del)(void *));

/**
 * @brief	Applies a function to each element of a linked list.
 *
 * 			Iterates through the list 'lst' and applies the function 'f'
 *			to the content of each node.
 *
 * @param	lst	Pointer to the first node of the list.
 * @param	f	Function to apply to the content of each node.
 *
 * @note	Does nothing if 'lst' is NULL.
 */
void		ft_lstiter(t_list *lst, void (*f)(void *));

/**
 * @brief	Returns the last node of a linked list.
 *
 * 			Traverses the list 'lst' and returns a pointer to the last node.
 *			If the list is empty, returns NULL.
 *
 * @param	lst	Pointer to the first node of the list.
 *
 * @return	Pointer to the last node of the list, or NULL if the list is
 *			empty.
 */
t_list		*ft_lstlast(t_list *lst);

/**
 * @brief	Creates a new list by applying a function to each element.
 *
 * 			Iterates through 'lst' and applies the function 'f' to the
 *			content of each node. Creates a new list from the results.
 *			If allocation fails, uses 'del' to free content and clears
 *			the new list.
 *
 * @param	lst	Pointer to the first node of the original list.
 * @param	f	Function to apply to the content of each node.
 * @param	del	Function used to free content in case of allocation failure.
 *
 * @note	The original list is not modified.
 * @note	If 'lst', 'f', or 'del' is NULL, returns NULL.
 *
 * @return	Pointer to the newly created list, or NULL on failure.
 */
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/**
 * @brief	Creates a new node for a linked list.
 *
 * 			Allocates memory for a new list node, assigns 'content' to it,
 *			and sets the 'next' pointer to NULL.
 *
 * @param	content	Pointer to the content to store in the new node.
 *
 * @return	Pointer to the newly created node, or NULL if allocation fails.
 */
t_list		*ft_lstnew(void *content);

/**
 * @brief	Counts the number of nodes in a linked list.
 *
 * 			Traverses the list 'lst' and returns the total number of nodes
 *			it contains.
 *
 * @param	lst	Pointer to the first node of the list.
 *
 * @return	Number of nodes in the list.
 */
int			ft_lstsize(t_list *lst);

/**
 * @brief	Searches for a byte in a memory block.
 *
 * 			Scans the first 'n' bytes of the memory area pointed to by 's'
 *			for the first occurrence of byte 'c'.
 *
 * @param	s	Pointer to the memory area to search.
 * @param	c	Byte to search for (converted to unsigned char).
 * @param	n	Number of bytes to examine.
 *
 * @return	Pointer to the first occurrence of 'c' in 's', or NULL if not
 *			found within the first 'n' bytes.
 */
void		*ft_memchr(const void *s, int c, size_t n);

/**
 * @brief	Compares two memory blocks byte by byte.
 *
 * 			Compares the first 'n' bytes of the memory areas 's1' and 's2'.
 *			Returns the difference between the first pair of differing bytes.
 *
 * @param	s1	Pointer to the first memory block.
 * @param	s2	Pointer to the second memory block.
 * @param	n	Number of bytes to compare.
 *
 * @note	If either 's1' or 's2' is NULL, returns 'n'.
 *
 * @return	Integer less than, equal to, or greater than zero if 's1' is
 *			respectively less than, equal to, or greater than 's2'.
 */
int			ft_memcmp(const void *s1, const void *s2, size_t n);

/**
 * @brief	Copies 'n' bytes from source to destination memory.
 *
 * 			Copies 'n' bytes from the memory area 'src' to 'dest'. If both
 *			'src' and 'dest' are NULL, does nothing. Internally uses
 *			ft_memmove to handle the copy.
 *
 * @param	dest	Pointer to the destination memory area.
 * @param	src		Pointer to the source memory area.
 * @param	n		Number of bytes to copy.
 *
 * @return	Pointer to 'dest'.
 */
void		*ft_memcpy(void *dest, const void *src, size_t n);

/**
 * @brief	Copies 'n' bytes from source to destination safely.
 *
 * 			Copies 'n' bytes from the memory area 'src' to 'dest'. Handles
 *			overlapping memory regions correctly to prevent data corruption.
 *
 * @param	dest	Pointer to the destination memory area.
 * @param	src		Pointer to the source memory area.
 * @param	n		Number of bytes to copy.
 *
 * @return	Pointer to 'dest'.
 */
void		*ft_memmove(void *dest, const void *src, size_t n);

/**
 * @brief	Fills a memory area with a constant byte.
 *
 * 			Sets the first 'n' bytes of the memory area pointed to by 's'
 *			to the byte value 'c'.
 *
 * @param	s	Pointer to the memory area to fill.
 * @param	c	Byte value to set (converted to unsigned char).
 * @param	n	Number of bytes to set.
 *
 * @return	Pointer to the memory area 's'.
 */
void		*ft_memset(void *s, int c, size_t n);

/**
 * @brief	Prints formatted output similar to standard printf.
 *
 *			Supports %c, %s, %d, %i, %u, %x, %X, %p, and %% specifiers.
 *			Parses flags, width, and precision. Allocates memory for
 *			intermediate strings, prints formatted output, and returns
 *			total characters printed.
 *
 * @param	s	Format string containing text and format specifiers.
 * @param	...	Variable arguments to format and print.
 *
 * @return	Total number of characters printed, or -1 on error.
 */
int			ft_printf(char const *s, ...);

/**
 * @brief	Writes a character to a given file descriptor.
 *
 * 			Sends the character 'c' to the file or output stream
 *			associated with the file descriptor 'fd'.
 *
 * @param	c	Character to write.
 * @param	fd	File descriptor where the character will be written.
 *
 * @return	Number of bytes written, or -1 on error.
 */
int			ft_putchar_fd(char c, int fd);

/**
 * @brief	Writes a string followed by a newline to a file descriptor.
 *
 * 			Sends the string 's' to the file or output stream associated
 *			with 'fd', then writes a newline character.
 *
 * @param	s	String to write.
 * @param	fd	File descriptor where the string will be written.
 *
 * @return	Total number of bytes written, or -1 on error.
 */
int			ft_putendl_fd(char *s, int fd);

/**
 * @brief	Writes an integer to a given file descriptor.
 *
 * 			Sends the integer 'n' as a sequence of characters to the file
 *			or output stream associated with 'fd'. Handles negative numbers
 *			and the special case of INT_MIN.
 *
 * @param	n	Integer to write.
 * @param	fd	File descriptor where the number will be written.
 *
 * @return	Total number of bytes written, or -1 on error.
 */
int			ft_putnbr_fd(int n, int fd);

/**
 * @brief	Writes a string to a given file descriptor.
 *
 * 			Sends the string 's' to the file or output stream associated
 *			with the file descriptor 'fd'.
 *
 * @param	s	String to write.
 * @param	fd	File descriptor where the string will be written.
 *
 * @return	Number of bytes written, or -1 on error.
 */
int			ft_putstr_fd(char *s, int fd);

/**
 * @brief	Reads all lines from a file descriptor into a linked list.
 *
 * 			Reads lines from the file descriptor 'fd' using get_next_line
 *			and stores each line as a new node in the linked list 'lines'.
 *			Frees all allocated memory and returns -1 if allocation fails.
 *
 * @param	lines	Pointer to the pointer of the first node of the list.
 * @param	fd		File descriptor to read from.
 *
 * @return	Number of lines read, or -1 on allocation failure.
 */
int			ft_read_file(t_list **lines, int fd);

/**
 * @brief	Resizes a memory block, preserving its content.
 *
 * 			Allocates a new memory block of size 'new_size', copies the
 *			content from 'ptr' up to the smaller of 'old_size' or
 *			'new_size', and frees the old block. Handles NULL pointers and
 *			zero-size allocations.
 *
 * @param	ptr		Pointer to the old memory block.
 * @param	new_size	New size in bytes for the memory block.
 * @param	old_size	Old size in bytes of the memory block.
 *
 * @return	Pointer to the new memory block, or NULL on allocation failure.
 */
void		*ft_realloc(void *ptr, size_t new_size, size_t old_size);

/**
 * @brief	Splits a string into an array of strings using a delimiter.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c'.
 *			Returns a NULL-terminated array of strings.
 *
 *			Static helpers:
 *
 *				- wrdcount():	Counts the number of words in 's' separated
 *								by 'c'.
 *
 *				- wrdlen():		Calculates the length of a word in 's' until
 *								the next 'c' or string end.
 *
 * @param	s	String to split.
 * @param	c	Delimiter character.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char		**ft_split(char const *s, char c);

/**
 * @brief	Splits a string into an array of strings by a delimiter.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c',
 *			while respecting quoted substrings (single and double quotes).
 *			Returns a NULL-terminated array of strings.
 *
 *			Static helpers:
 *
 *				- wrdlen():		Calculates the length of a word considering
 *								quotes.
 *
 *				- wrdcount():	Counts the number of words in 's' considering
 *								quotes.
 *
 * @param	s	String to split.
 * @param	c	Delimiter character.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char		**ft_split_iq(char const *s, char c);

/**
 * @brief	Splits a string using a delimiter and a set of special strings.
 *
 * 			Splits the string 's' at each occurrence of the delimiter 'c',
 *			while respecting quoted substrings, and at each special sets 
 *			string defined in 'base'. Returns a NULL-terminated array of
 *			strings.
 *
 *			Static helpers:
 *
 *				- strchr_iqreg():	Finds the next split position for word
 *									in 's' considering quoted substrings.
 *
 *				- strchr_iqset():	Finds the next split position for base word
 *									in 's' considering quoted substrings.
 *
 *				- wrd_count():		Counts words in 's' based on 'c' and 'base'.
 *
 *				- create_split():	Creates and fills the array of strings.
 *
 * @param	s		String to split.
 * @param	c		Delimiter character.
 * @param	base	Array of special strings to consider for splitting.
 *
 * @note	Memory is allocated for the returned array and its strings.
 *			Caller must free it using a function like ft_split_free.
 *
 * @return	NULL-terminated array of strings, or NULL on allocation failure.
 */
char		**ft_split_iqbase(const char *s, char c, const char **base);

/**
 * @brief	Frees a NULL-terminated array of strings.
 *
 * 			Frees each string in the array 'split' and then frees the array
 *			itself. Returns NULL for convenience in assignment.
 *
 * @param	split	Array of strings to free.
 *
 * @return	NULL.
 */
void		*ft_split_free(char **split);

/**
 * @brief	Finds the first occurrence of a character in a string.
 *
 * 			Searches for the first occurrence of the character 'c' in the
 *			string 's'. Returns a pointer to the character found or NULL if
 *			not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to search for.
 *
 * @return	Pointer to the first occurrence of 'c' in 's', or NULL if not found.
 */
char		*ft_strchr(const char *s, int c);

/**
 * @brief	Finds a character in a string, considering quoted substrings.
 *
 * 			Searches for the first occurrence of character 'c' in the
 *			string 's', skipping characters inside single or double quotes.
 *			Returns a pointer to the character found or NULL if not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to search for.
 *
 * @return	Pointer to the first occurrence of 'c' outside quotes, or NULL.
 */
char		*ft_strchr_iq(char const *s, char c);

/**
 * @brief	Creates a duplicate of a string.
 *
 * 			Allocates memory and copies the content of the string 's' into
 *			the new memory. Returns a pointer to the duplicated string.
 *
 * @param	s	String to duplicate.
 *
 * @return	Pointer to the duplicated string, or NULL on allocation failure.
 */
char		*ft_strdup(const char *s);

/**
 * @brief	Applies a function to each character of a string.
 *
 * 			Iterates over the string 's' and applies the function 'f' to
 *			each character. The function receives the character's index and
 *			a pointer to the character itself.
 *
 * @param	s	String to iterate over.
 * @param	f	Function to apply to each character and its index.
 *
 * @note	The function 'f' can modify the characters in the string directly.
 */
void		ft_striteri(char *s, void (*f)(unsigned int, char *));

/**
 * @brief	Concatenates two strings into a newly allocated string.
 *
 * 			Creates a new string that contains the contents of 's1' followed
 *			by the contents of 's2'. Memory is allocated for the new string.
 *
 * @param	s1	First string.
 * @param	s2	Second string.
 *
 * @return	Pointer to the concatenated string, or NULL on allocation failure.
 */
char		*ft_strjoin(char const *s1, char const *s2);

/**
 * @brief	Appends a string to another, respecting buffer size.
 *
 * 			Appends the string 'src' to the end of 'dst', ensuring that
 *			at most 'size - 1' characters are written and the result is
 *			NULL-terminated. Returns the total length of the string it tried
 *			to create.
 *
 * @param	dst	Destination buffer.
 * @param	src	Source string to append.
 * @param	size	Size of the destination buffer.
 *
 * @return	Total length of the string it tried to create (dst + src length).
 */
size_t		ft_strlcat(char *dst, const char *src, size_t size);

/**
 * @brief	Copies a string into a buffer, respecting its size.
 *
 * 			Copies up to 'size - 1' characters from 'src' into 'dst' and
 *			NULL-terminates the result if 'size' is not zero. Returns the
 *			length of 'src'.
 *
 * @param	dst	Destination buffer.
 * @param	src	Source string to copy.
 * @param	size	Size of the destination buffer.
 *
 * @return	Length of the source string 'src'.
 */
size_t		ft_strlcpy(char *dst, const char *src, size_t size);

/**
 * @brief	Calculates the length of a string.
 *
 * 			Counts the number of characters in the string 's' until the
 *			NULL terminator. Returns 0 if 's' is NULL.
 *
 * @param	s	String to measure.
 *
 * @return	Number of characters in 's', excluding the NULL terminator.
 */
size_t		ft_strlen(const char *s);

/**
 * @brief	Calculates the length of a string until a given character.
 *
 * 			Counts the number of characters in 'str' until character 'c'
 *			or the NULL terminator is found.
 *
 * @param	str	String to measure.
 * @param	c	Character to stop counting at.
 *
 * @return	Number of characters before 'c' or end of string.
 */
int			ft_strlen_chr(char *str, char c);

/**
 * @brief	Creates a new string by applying a function to each character.
 *
 * 			Allocates a new string where each character is the result of
 *			applying the function 'f' to the corresponding character of 's'.
 *
 * @param	s	String to map.
 * @param	f	Function to apply to each character and its index.
 *
 * @return	New string with mapped characters, or NULL on allocation failure.
 */
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));

/**
 * @brief	Compares up to 'n' characters of two strings.
 *
 * 			Compares the strings 's1' and 's2' up to 'n' characters or until
 *			a difference or NULL terminator is found. Returns the difference
 *			between the first differing characters.
 *
 * @param	s1	First string to compare.
 * @param	s2	Second string to compare.
 * @param	n	Maximum number of characters to compare.
 *
 * @return	Integer less than, equal to, or greater than zero if 's1' is
 *			less than, equal to, or greater than 's2'.
 */
int			ft_strncmp(const char *s1, const char *s2, size_t n);

/**
 * @brief	Searches for a substring within a limited length of a string.
 *
 * 			Searches for the first occurrence of 'little' in 'big', but
 *			only within the first 'len' characters of 'big'. Returns a
 *			pointer to the beginning of 'little' in 'big', or NULL if not
 *			found.
 *
 * @param	big	String to search within.
 * @param	little	Substring to search for.
 * @param	len	Maximum number of characters to search in 'big'.
 *
 * @return	Pointer to the first occurrence of 'little' in 'big', or NULL.
 */
char		*ft_strnstr(const char *big, const char *little, size_t len);

/**
 * @brief	Finds the last occurrence of a character in a string.
 *
 * 			Searches for the last occurrence of the character 'c' in the
 *			string 's'. Returns a pointer to this character, or NULL if it
 *			is not found.
 *
 * @param	s	String to search in.
 * @param	c	Character to locate.
 *
 * @return	Pointer to the last occurrence of 'c' in 's', or NULL if none.
 */
char		*ft_strrchr(const char *s, int c);

/**
 * @brief	Trims all characters in 'set' from start and end of 's1'.
 *
 *			Allocates and returns a new string with all leading and
 *			trailing characters found in 'set' removed from 's1'.
 *			Returns an empty string if 's1' only contains characters
 *			from 'set'.
 *
 *			Static helpers:
 *
 *				- istrim():		Checks if a character exists in the set.
 *
 * @param	s1		String to trim.
 * @param	set		String containing characters to remove from 's1'.
 *
 * @return	New trimmed string, empty string if all chars trimmed, or
 *			NULL on allocation failure or invalid input.
 */
char		*ft_strtrim(char const *s1, char const *set);

/**
 * @brief	Extracts a substring from a given string.
 *
 *			Allocates and returns a new string that is a substring of
 *			's', starting at index 'start' and up to 'len' characters.
 *			If 'start' is beyond the end of 's', returns an empty string.
 *			If 'len' exceeds the remaining string length, it is reduced.
 *
 * @param	s		Original string to extract from.
 * @param	start	Starting index for the substring.
 * @param	len		Maximum number of characters to include.
 *
 * @note	Returns NULL if 's' is NULL or memory allocation fails.
 * @note	The returned string must be freed by the caller.
 *
 * @return	Newly allocated substring, empty string if start > strlen(s),
 *			or NULL on allocation failure.
 */
char		*ft_substr(char const *s, unsigned int start, size_t len);

/**
 * @brief	Converts an uppercase letter to lowercase.
 *
 *			If 'c' is an uppercase ASCII letter ('A' to 'Z'), returns
 *			the corresponding lowercase letter. All other characters
 *			are returned unchanged.
 *
 * @param	c	Character to convert.
 *
 * @return	Lowercase equivalent if 'c' is uppercase, else 'c' unchanged.
 */
int			ft_tolower(int c);

/**
 * @brief	Converts a lowercase letter to uppercase.
 *
 *			If 'c' is a lowercase ASCII letter ('a' to 'z'), returns
 *			the corresponding uppercase letter. All other characters
 *			are returned unchanged.
 *
 * @param	c	Character to convert.
 *
 * @return	Uppercase equivalent if 'c' is lowercase, else 'c' unchanged.
 */
int			ft_toupper(int c);

/**
 * @brief	Converts an unsigned integer to a string in a given base.
 *
 *			Converts the number 'n' to a string representation using
 *			the symbols provided in 'base'. The returned string is
 *			dynamically allocated and null-terminated.
 *
 *			Static helpers:
 *
 *				- ft_nbrlen():	Calculates the number of digits needed
 *								to represent 'n' in the given base.
 *
 * @param	n		Unsigned integer to convert.
 * @param	base	String containing all symbols of the target base.
 *
 * @note	The returned string must be freed by the caller.
 * @note	'base' length determines the radix (e.g., 2 for binary, 16 for hex).
 *
 * @return	Dynamically allocated string representing 'n' in the given base,
 *			or NULL if memory allocation fails.
 */
char		*ft_utoa_base(size_t n, const char *base);

/*--------------------------------------------------------------------------*/
/*------------------------------------END-----------------------------------*/
/*--------------------------------------------------------------------------*/

#endif
