#	================================ Main Configuration ================================	#

#	Library name
NAME = ft

# 	Source files
SOURCES = \
	$(SOURCE_DIR)/ft_argjoin.c 				\
	$(SOURCE_DIR)/ft_arradd_str.c 			\
	$(SOURCE_DIR)/ft_arrlen.c 				\
	$(SOURCE_DIR)/ft_atoi_base.c			\
	$(SOURCE_DIR)/ft_atoi.c					\
	$(SOURCE_DIR)/ft_atol.c 				\
	$(SOURCE_DIR)/ft_bzero.c 				\
	$(SOURCE_DIR)/ft_calloc.c 				\
	$(SOURCE_DIR)/ft_fprintf.c 				\
	$(SOURCE_DIR)/ft_get_next_line_utils.c	\
	$(SOURCE_DIR)/ft_get_next_line.c		\
	$(SOURCE_DIR)/ft_isalnum.c				\
	$(SOURCE_DIR)/ft_isascii.c				\
	$(SOURCE_DIR)/ft_isalpha.c				\
	$(SOURCE_DIR)/ft_isdigit.c				\
	$(SOURCE_DIR)/ft_isprint.c				\
	$(SOURCE_DIR)/ft_itoa.c					\
	$(SOURCE_DIR)/ft_lstadd_back_bonus.c	\
	$(SOURCE_DIR)/ft_lstadd_front_bonus.c	\
	$(SOURCE_DIR)/ft_lstclear_bonus.c		\
	$(SOURCE_DIR)/ft_lstdelone_bonus.c		\
	$(SOURCE_DIR)/ft_lstiter_bonus.c		\
	$(SOURCE_DIR)/ft_lstlast_bonus.c		\
	$(SOURCE_DIR)/ft_lstmap_bonus.c			\
	$(SOURCE_DIR)/ft_lstnew_bonus.c			\
	$(SOURCE_DIR)/ft_lstsize_bonus.c		\
	$(SOURCE_DIR)/ft_memchr.c				\
	$(SOURCE_DIR)/ft_memcmp.c				\
	$(SOURCE_DIR)/ft_memcpy.c				\
	$(SOURCE_DIR)/ft_memmove.c				\
	$(SOURCE_DIR)/ft_memset.c				\
	$(SOURCE_DIR)/ft_printf_dup_utils.c		\
	$(SOURCE_DIR)/ft_printf_flg_utils.c		\
	$(SOURCE_DIR)/ft_printf_utils.c			\
	$(SOURCE_DIR)/ft_printf.c				\
	$(SOURCE_DIR)/ft_putchar_fd.c			\
	$(SOURCE_DIR)/ft_putendl_fd.c			\
	$(SOURCE_DIR)/ft_putnbr_fd.c			\
	$(SOURCE_DIR)/ft_putstr_fd.c			\
	$(SOURCE_DIR)/ft_read_file.c			\
	$(SOURCE_DIR)/ft_realloc.c				\
	$(SOURCE_DIR)/ft_split_free.c			\
	$(SOURCE_DIR)/ft_split_iq.c				\
	$(SOURCE_DIR)/ft_split_iqbase.c			\
	$(SOURCE_DIR)/ft_split_utils.c			\
	$(SOURCE_DIR)/ft_split.c				\
	$(SOURCE_DIR)/ft_strchr_ip.c			\
	$(SOURCE_DIR)/ft_strchr_iq.c			\
	$(SOURCE_DIR)/ft_strchr.c				\
	$(SOURCE_DIR)/ft_strdup.c				\
	$(SOURCE_DIR)/ft_striteri.c				\
	$(SOURCE_DIR)/ft_strjoin.c				\
	$(SOURCE_DIR)/ft_strlcat.c				\
	$(SOURCE_DIR)/ft_strlcpy.c				\
	$(SOURCE_DIR)/ft_strlen_chr.c			\
	$(SOURCE_DIR)/ft_strlen.c				\
	$(SOURCE_DIR)/ft_strmapi.c				\
	$(SOURCE_DIR)/ft_strncmp.c				\
	$(SOURCE_DIR)/ft_strnstr_ip.c			\
	$(SOURCE_DIR)/ft_strnstr.c				\
	$(SOURCE_DIR)/ft_strrchr.c				\
	$(SOURCE_DIR)/ft_strtrim.c				\
	$(SOURCE_DIR)/ft_substr.c				\
	$(SOURCE_DIR)/ft_tolower.c				\
	$(SOURCE_DIR)/ft_toupper.c				\
	$(SOURCE_DIR)/ft_utoa_base.c

#	================================ Directory Structure ===============================	#

#	Source, Object, and Dependency directories names
SOURCE_DIR = src
OBJECT_DIR = obj
DEPEND_DIR = dep

#	Include paths for headers
INCLUDE_DIRS = -I include

#	================================ Compiler Settings =================================	#

#	Compiler flags
CFLAGS = -Wall -Wextra -Werror -g

#	Dependency generation flags
DEPFLAGS = -MMD -MP

#	Preprocessor flags (includes)
CPPFLAGS = $(INCLUDE_DIRS)

#	================================ Object Files ======================================	#

#	Object files
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

#	Dependency files
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.c=$(DEPEND_DIR)/%.d)

#	================================ Colors for Output =================================	#

COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_YELLOW = \033[0;33m
COLOR_BLUE = \033[0;34m
COLOR_CYAN = \033[0;35m
COLOR_RESET = \033[0m

#	================================ Build Rules =======================================	#

.PHONY: all msg clean fclean re

#	Default: build library
all: $(NAME)

#	Display compilation message
msg:
	@echo "$(COLOR_BLUE)[i] $(NAME) - Object compilation:$(COLOR_RESET)"

#	================================ Directory Creation ================================	#

#	Create necessary directories
$(OBJECT_DIR) $(DEPEND_DIR):
	@mkdir -p $(DEPEND_DIR)
	@mkdir -p $(OBJECT_DIR)

#	================================ Object Compilation ================================	#

#	Compile source files to object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(DEPEND_DIR) $(OBJECT_DIR)
	@printf "\t$(COLOR_CYAN)[OK] Compiling %-30s\r$(COLOR_RESET)" $<
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@ -MF $(DEPEND_DIR)/$(@F:.o=.d)

#	Include dependency files for header tracking
-include $(DEPENDENCIES)

#	================================ Library Creation ==================================	#

#	Create static library archive
$(NAME): msg $(OBJECT_DIR) $(OBJECTS)
	@echo "\r\t$(COLOR_CYAN)[OK] Objects compiled successfully.$(COLOR_RESET)"
	@ar rcs lib$(NAME).a $(OBJECTS)
	@echo "$(COLOR_BLUE)[✓] $(NAME) - Library built successfully.$(COLOR_RESET)\n"

#	================================ Cleanup Rules =====================================	#

#	Remove object and dependency files
clean:
	@rm -rf $(OBJECT_DIR) $(DEPEND_DIR)
	@echo "$(COLOR_YELLOW)[!] $(NAME) - Cleaned object files.$(COLOR_RESET)"

#	Remove everything including library
fclean: clean
	@rm -f lib$(NAME).a
	@echo "$(COLOR_RED)[x] $(NAME) - Fully cleaned project files.$(COLOR_RESET)\n"

#	Rebuild from scratch
re: fclean all

#	================================ Makefile End ======================================	#