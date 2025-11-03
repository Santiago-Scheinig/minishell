# ================================ Main Configuration ================================ #

# Executable names
NAME = msh
NAME_BONUS = msh_bonus

# Source files
MAIN_SRC = $(SOURCE_DIR)/main.c
BONUS_SRC = $(SOURCE_DIR)/main_bonus.c

# ================================ Library Configuration ============================= #

# Libraries to build (in build order)
LIBS = ft shellft mshft

# Bonus libraries (if needed)
BONUS_LIBS = 

# Library linking order: most dependent first, base libraries last
# msh depends on shellft, shellft depends on ft
LINK_LIBS = -lmshft -lshellft -lft -lreadline

# ================================ Directory Structure =============================== #

SOURCE_DIR = src
OBJECT_DIR = obj
DEPEND_DIR = dep

# Include paths for headers
INCLUDE_DIRS = -I src/mshft/include -I src/shellft/include -I src/ft/include

# Library paths for linking
LIBRARY_PATHS = $(addprefix -L $(SOURCE_DIR)/, $(LIBS))

# ================================ Compiler Settings ================================= #

# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g

# Dependency generation flags
DEPFLAGS = -MMD -MP

# Preprocessor flags (includes)
CPPFLAGS = $(INCLUDE_DIRS)

# Linker flags (library paths)
LDFLAGS = $(LIBRARY_PATHS)

# ================================ Object Files ====================================== #

# Main program objects
MAIN_OBJ = $(MAIN_SRC:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

# Bonus program objects
BONUS_OBJ = $(BONUS_SRC:$(SOURCE_DIR)/%.c=$(OBJECT_DIR)/%.o)

# Dependency files
ALL_SOURCES = $(MAIN_SRC) $(BONUS_SRC)
DEPENDENCIES = $(ALL_SOURCES:$(SOURCE_DIR)/%.c=$(DEPEND_DIR)/%.d)

# ================================ Colors for Output ================================= #

COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;34m
COLOR_CYAN = \033[0;35m
COLOR_RESET = \033[0m

# ================================ Build Rules ======================================= #

.PHONY: all bonus clean fclean re

# Default: build main program
all: $(NAME)
	@echo "$(COLOR_GREEN)[✓] $(NAME) - Build complete!$(COLOR_RESET)"

msg:
	@echo "$(COLOR_BLUE)[i] $(NAME) - Main Compilation:"

# Build bonus program
bonus: $(NAME_BONUS)
	@echo "$(COLOR_GREEN)[✓] $(NAME_BONUS) - Bonus build complete!$(COLOR_RESET)"

# ================================ Directory Creation ================================ #

$(OBJECT_DIR) $(DEPEND_DIR):
	@mkdir -p $@

# ================================ Library Building ================================== #

# Build each library in its subdirectory
$(LIBS):
	@$(MAKE) -s -C $(SOURCE_DIR)/$@

# ================================ Object Compilation ================================ #

# Compile source files to object files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c | $(DEPEND_DIR) $(OBJECT_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ -MF $(DEPEND_DIR)/$(@F:.o=.d)
	@printf "\r\t$(COLOR_GREEN)[OK] Compiled $<$(COLOR_RESET)"

# Include dependency files for header tracking
-include $(DEPENDENCIES)

# ================================ Linking =========================================== #

# Link main executable
# Order: compiler, object files, library paths, libraries, output
$(NAME): $(LIBS) msg $(MAIN_OBJ)
	@printf "\r\t$(COLOR_CYAN)[OK] Main compiled successfully.\n$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(MAIN_OBJ) $(LDFLAGS) $(LINK_LIBS) -o $@
	@echo "$(COLOR_BLUE)[✓] $(NAME) - Linked successfully$(COLOR_RESET)\n"

# Link bonus executable
$(NAME_BONUS): $(LIBS) msg $(BONUS_OBJ)
	@$(CC) $(CFLAGS) $(BONUS_OBJ) $(LDFLAGS) $(LINK_LIBS) -o $@
	@echo "$(COLOR_BLUE)[✓] $(NAME_BONUS) - Linked successfully$(COLOR_RESET)\n"

# ================================ Cleanup Rules ===================================== #

# Remove object and dependency files
clean:
	@$(foreach lib, $(LIBS), $(MAKE) -s -C $(SOURCE_DIR)/$(lib) clean;)
	@rm -rf $(OBJECT_DIR) $(DEPEND_DIR)
	@echo "$(COLOR_BLUE)[i] Cleaned build artifacts$(COLOR_RESET)"

# Remove everything including executables
fclean:
	@$(foreach lib, $(LIBS), $(MAKE) -s -C $(SOURCE_DIR)/$(lib) fclean;)
	@rm -f $(NAME) $(NAME_BONUS)
	@echo "$(COLOR_RED)[x] Full clean complete$(COLOR_RESET)\n"

# Rebuild from scratch
re: fclean all