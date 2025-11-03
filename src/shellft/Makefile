#	================================ Main Configuration ================================	#

#	Library name
NAME = shellft

#	Source files - shell utility functions
SOURCES = \
	$(SOURCE_DIR)/shell_env_utils.c 	\
	$(SOURCE_DIR)/shell_envarr_mod.c 	\
	$(SOURCE_DIR)/shell_envchr.c 		\
	$(SOURCE_DIR)/shell_envdup.c 		\
	$(SOURCE_DIR)/shell_envdup_lst.c 	\
	$(SOURCE_DIR)/shell_envini.c 		\
	$(SOURCE_DIR)/shell_envlst_mod.c 	\
	$(SOURCE_DIR)/shell_envsort.c 		\
	$(SOURCE_DIR)/shell_lastcmd_upd.c 	\
	$(SOURCE_DIR)/shell_lstadd_newtkn.c	\
	$(SOURCE_DIR)/shell_memmove.c 		\
	$(SOURCE_DIR)/shell_newenv.c 		\
	$(SOURCE_DIR)/shell_newpmt.c		\
	$(SOURCE_DIR)/shell_newpmt_utils.c 	\
	$(SOURCE_DIR)/shell_newtkn.c 		\
	$(SOURCE_DIR)/shell_sigdlr.c 		\
	$(SOURCE_DIR)/shell_sigdlr_utils.c 	\
	$(SOURCE_DIR)/shell_split.c 		\
	$(SOURCE_DIR)/shell_split_chr.c 	\
	$(SOURCE_DIR)/shell_split_utils.c 	\
	$(SOURCE_DIR)/shell_tsh.c 			\
	$(SOURCE_DIR)/shell_tsh_del.c

#	================================ Directory Structure ===============================	#

#	Source, Object, and Dependency directories names
SOURCE_DIR = src
OBJECT_DIR = obj
DEPEND_DIR = dep

#	Include paths for headers
INCLUDE_DIRS = -I include -I ../ft/include

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
	@printf "\r\t$(COLOR_CYAN)[OK] Compiling %-30s\r$(COLOR_RESET)" $<
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