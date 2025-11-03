#	================================ Main Configuration ================================	#

#	Library name
NAME = mshft

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

#	================================ Directory Structure ===============================	#

#	Source, Object, and Dependency directories names
SOURCE_DIR = src
OBJECT_DIR = obj
DEPEND_DIR = dep

#	Include paths for headers
INCLUDE_DIRS = -I include -I ../ft/include -I ../shellft/include

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

.PHONY: all msg bonus clean fclean re

#	Default: build library
all: $(NAME)

#	Display compilation message
msg:
	@echo "$(COLOR_BLUE)[i] $(NAME) - Object compilation:$(COLOR_RESET)"

#	================================ Directory Creation ================================	#

#	Create necessary directories including subdirectories
$(OBJECT_DIR) $(DEPEND_DIR):
	@mkdir -p $(DEPEND_DIR)
	@mkdir -p $(OBJECT_DIR)/bin
	@mkdir -p $(OBJECT_DIR)/execution
	@mkdir -p $(OBJECT_DIR)/parser

#	================================ Object Compilation ================================	#

#	Compile source files to object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(DEPEND_DIR) $(OBJECT_DIR)
	@printf "\r\t$(COLOR_CYAN)[OK] Compiling %-40s\r$(COLOR_RESET)" $<
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(CPPFLAGS) -c $< -o $@ -MF $(DEPEND_DIR)/$(@F:.o=.d)

#	Include dependency files for header tracking
-include $(DEPENDENCIES)

#	================================ Library Creation ==================================	#

#	Create static library archive
$(NAME): msg $(OBJECT_DIR) $(OBJECTS)
	@printf "\r\t$(COLOR_CYAN)[OK] Objects compiled successfully.          \n$(COLOR_RESET)"
	@ar rcs lib$(NAME).a $(OBJECTS)
	@echo "$(COLOR_BLUE)[✓] $(NAME) - Library built successfully.$(COLOR_RESET)\n"

#	================================ Bonus Rules =======================================	#

bonus:
	@echo "$(COLOR_YELLOW)[i] Bonus target - not implemented$(COLOR_RESET)\n"

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