#	================================ Main Configuration ================================	#

#	Library name
NAME = mshft
NAME_BONUS = mshft_bonus

#	Source files organized by module
SOURCES = \
	$(SOURCE_DIR)/bin/bin_cd.c					\
	$(SOURCE_DIR)/bin/bin_echo.c				\
	$(SOURCE_DIR)/bin/bin_env.c					\
	$(SOURCE_DIR)/bin/bin_exit.c				\
	$(SOURCE_DIR)/bin/bin_export.c				\
	$(SOURCE_DIR)/bin/bin_import.c				\
	$(SOURCE_DIR)/bin/bin_pwd.c					\
	$(SOURCE_DIR)/bin/bin_unset.c				\
	$(SOURCE_DIR)/bin/bin_utils.c				\
	$(SOURCE_DIR)/execution/exebin.c			\
	$(SOURCE_DIR)/execution/execmd.c			\
	$(SOURCE_DIR)/execution/execution.c			\
	$(SOURCE_DIR)/execution/exetsh.c			\
	$(SOURCE_DIR)/execution/exeutils.c			\
	$(SOURCE_DIR)/execution/waitexec.c			\
	$(SOURCE_DIR)/parser/parser_cmd_utils.c		\
	$(SOURCE_DIR)/parser/parser_cmd.c			\
	$(SOURCE_DIR)/parser/parser_envar_utils.c	\
	$(SOURCE_DIR)/parser/parser_envar.c			\
	$(SOURCE_DIR)/parser/parser_input.c			\
	$(SOURCE_DIR)/parser/parser_token.c

SOURCES_BONUS = \
	$(SOURCE_DIR)/parser/parser_wildc_bonus.c		\
	$(SOURCE_DIR)/parser/parser_wildc_utils_bonus.c	\
	$(SOURCE_DIR)/parser/parser_logic_bonus.c		\
	$(SOURCE_DIR)/parser/parser_logic_utils_bonus.c 

#	================================ Directory Structure ===============================	#

#	Source, Object, and Dependency directories names
SOURCE_DIR = src
OBJECT_DIR = obj
DEPEND_DIR = dep

# Separate dirs for the bonus build to avoid clobbering normal build artifacts
OBJECT_DIR_BONUS = obj_bonus

#	Include paths for headers
INCLUDE_DIRS = -I include -I ../libft/include -I ../shellft/include

#	================================ Compiler Settings =================================	#

#	Bonus flag (1 to enable bonus features, 0 otherwise)
BFLAG ?= 0

#	Compiler flags
CFLAGS = -Wall -Wextra -Werror -g

#	Dependency generation flags
DEPFLAGS = -MMD -MP

#	Preprocessor flags (includes)
CPPFLAGS = $(INCLUDE_DIRS)

#	================================ Object Files ======================================	#

#	Object files for normal build (obj/...)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

#	Object files for bonus build (obj_bonus/...)
BONUS_OBJECTS = $(SOURCES_BONUS:$(SOURCE_DIR)/%.c=$(OBJECT_DIR_BONUS)/%.o) \
				$(SOURCES:$(SOURCE_DIR)/%.c=$(OBJECT_DIR_BONUS)/%.o)

#	Dependency files (both builds)
DEPENDENCIES = $(DEPEND_DIR)/*.d $(DEPEND_DIR_BONUS)/*.d

#	================================ Colors for Output =================================	#

COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_YELLOW = \033[0;33m
COLOR_BLUE = \033[0;34m
COLOR_CYAN = \033[0;35m
COLOR_RESET = \033[0m

#	================================ Build Rules =======================================	#

.PHONY: all msg bonus clean fclean re

#	Default: build library (normal)
all: $(NAME)

#	Display compilation message
msg:
	@echo "$(COLOR_BLUE)[i] $(NAME) - Object compilation:$(COLOR_RESET)"

msg_bonus:
	@echo "$(COLOR_BLUE)[i] $(NAME_BONUS) - Object compilation:$(COLOR_RESET)"

#	================================ Directory Creation ================================	#

#	Create necessary directories (both normal and bonus)
$(OBJECT_DIR) $(DEPEND_DIR) $(OBJECT_DIR_BONUS):
	@mkdir -p $(DEPEND_DIR)
	@mkdir -p $(OBJECT_DIR)/bin $(OBJECT_DIR)/execution $(OBJECT_DIR)/parser
	@mkdir -p $(OBJECT_DIR_BONUS)/bin $(OBJECT_DIR_BONUS)/execution $(OBJECT_DIR_BONUS)/parser

#	================================ Object Compilation ================================	#

#	Compile source files to object files (normal build, DBONUS=0)
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR) $(DEPEND_DIR)
	@printf "\r\t$(COLOR_CYAN)[OK] Compiling %-40s\r$(COLOR_RESET)" $<
	@$(CC) -DBONUS=0 $(CFLAGS) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@ -MF $(DEPEND_DIR)/$(@F:.o=.d)

#	Compile source files to object files (bonus build, DBONUS=1) - separate output dirs
$(OBJECT_DIR_BONUS)/%.o: $(SOURCE_DIR)/%.c | $(OBJECT_DIR_BONUS) $(DEPEND_DIR_BONUS)
	@printf "\r\t$(COLOR_CYAN)[OK] Compiling %-60s\r$(COLOR_RESET)" $<
	@$(CC) -DBONUS=1 $(CFLAGS) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@ -MF $(DEPEND_DIR)/$(@F:.o=.d)

#	Include dependency files for header tracking (both builds; safe if none exist)
-include $(wildcard $(DEPEND_DIR)/*.d) $(wildcard $(DEPEND_DIR_BONUS)/*.d)

#	================================ Library Creation ==================================	#

#	Create static library archive for normal build
$(NAME): msg $(OBJECT_DIR) $(OBJECTS)
	@printf "\r\t$(COLOR_CYAN)[OK] Objects compiled successfully.%-40s\n$(COLOR_RESET)"
	@ar rcs lib$(NAME).a $(OBJECTS)
	@echo "$(COLOR_BLUE)[✓] $(NAME) - Library built successfully.$(COLOR_RESET)\n"

bonus: msg_bonus $(OBJECT_DIR_BONUS) $(BONUS_OBJECTS)
	@printf "\r\t$(COLOR_CYAN)[OK] Objects compiled successfully.%-40s\n$(COLOR_RESET)"
	@ar rcs lib$(NAME_BONUS).a $(BONUS_OBJECTS)
	@echo "$(COLOR_BLUE)[✓] $(NAME_BONUS) - Library bonus built successfully.$(COLOR_RESET)\n"

#	================================ Cleanup Rules =====================================	#

#	Remove object and dependency files (both normal and bonus)
clean:
	@rm -rf $(OBJECT_DIR) $(DEPEND_DIR) $(OBJECT_DIR_BONUS) $(DEPEND_DIR_BONUS)
	@echo "$(COLOR_YELLOW)[!] $(NAME) - Cleaned object files.$(COLOR_RESET)"

#	Remove everything including both libraries
fclean: clean
	@rm -f lib$(NAME).a lib$(NAME_BONUS).a
	@echo "$(COLOR_RED)[x] $(NAME) - Fully cleaned project files.$(COLOR_RESET)\n"

#	Rebuild from scratch
re: fclean all

#	================================ Makefile End ======================================	#