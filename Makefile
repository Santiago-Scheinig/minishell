# -------------------------------- README --------------------------------- #

# This Makefile only works with programs executables by a main().

# The files where you have the main(), should be named main.c and		 	\
  main_bonus.c respectibly.

# ----------------------------- Makefile Main ----------------------------- #

# The name of the final executable.											#
NAME		=	minishell

# Set LIBFT_USE to 1 if you are using the libft for the project.			#
LIBFT_USE	:=	1

# The name list for regular and bonus source files.							#
SRC			=	$(SRC_DIR)/main.c								\
				$(SRC_DIR)/parser/parser.c						\
				$(SRC_DIR)/parser/parser_cmd.c					\
				$(SRC_DIR)/parser/parser_envar.c				\
				$(SRC_DIR)/parser/parser_token.c				\
				$(SRC_DIR)/parser/parser_token_utils.c			\
				$(SRC_DIR)/parser/parser_token_hdoc.c			\
				$(SRC_DIR)/parser/parser_cmd_utils.c			\
				$(SRC_DIR)/parser/parser_envar_utils.c			\
				$(SRC_DIR)/shellft/shell_split.c				\
				$(SRC_DIR)/shellft/shell_envdup.c				\
				$(SRC_DIR)/shellft/shell_memmove.c				\
				$(SRC_DIR)/shellft/shell_pmtstr.c				\
				$(SRC_DIR)/shellft/shell_pmtexp.c				\
				$(SRC_DIR)/shellft/shell_strchr.c				\
				$(SRC_DIR)/shellft/shell_getenv.c				\
				$(SRC_DIR)/shellft/shell_sortenv.c				\
				$(SRC_DIR)/shellft/shell_lstclear.c				\
				$(SRC_DIR)/shellft/shell_newlst_var.c			\
				$(SRC_DIR)/shellft/shell_split_utils.c			\
				$(SRC_DIR)/shellft/shell_addlst_token.c			\
				$(SRC_DIR)/shellft/shell_pmtexp_utils.c			\
				$(SRC_DIR)/shellft/shell_troubleshoot.c			\
				$(SRC_DIR)/shellft/shell_signal_utils.c			\
				$(SRC_DIR)/shellft/shell_signal_handlers.c		\
 				$(SRC_DIR)/execution/msh_cmd/msh_cd.c			\
 				$(SRC_DIR)/execution/msh_cmd/msh_env.c			\
 				$(SRC_DIR)/execution/msh_cmd/msh_pwd.c			\
 				$(SRC_DIR)/execution/msh_cmd/msh_echo.c			\
 				$(SRC_DIR)/execution/msh_cmd/msh_exit.c			\
 				$(SRC_DIR)/execution/msh_cmd/msh_unset.c		\
 				$(SRC_DIR)/execution/msh_cmd/msh_export.c		\
 				$(SRC_DIR)/execution/msh_cmd/msh_import.c		\
 				$(SRC_DIR)/execution/msh_cmd/msh_export_utils.c	\
 				$(SRC_DIR)/execution/waitcmd.c					\
 				$(SRC_DIR)/execution/exemsh.c					\
 				$(SRC_DIR)/execution/execmd.c					\
 				$(SRC_DIR)/execution/execmd_utils.c				\



SRC_BON		=	$(SRC_DIR)/main_bonus.c		\

# The flags used to compile the objects.									#
CFLAGS		=	-Wall -Wextra -Werror -g

# The flags used to create dependency files.								#
DFLAGS		=	-MMD -MP

# The preprocess flags used to compile the program.							#
CPPFLAGS	=	-I$(INC_DIR) -I $(INC_LIB)

# ----------------------------- Folder Names ------------------------------ #

# The directory name for source files.										#
SRC_DIR		=	src

# The directory name for object files.										#
OBJ_DIR		=	obj

# The directory name for library files.										#
LIB_DIR		=	lib

# The directory name for dependency files.									#
DEP_DIR		=	dep

# The directory name for index files.										#
INC_DIR		=	include

# The directory name for libft index files.									#
INC_LIB		=	src/libft/include

# --------------------------- Object Libraries ---------------------------- #

# The name of the regular project library.									#
LIB_REG		=	$(LIB_DIR)/$(NAME).a

# The name of the bonus project library.									#
LIB_BON		=	$(LIB_DIR)/$(NAME)_bonus.a

# The total name list for both regular and bonus source files.				#
ALL_SRC		=	$(SRC) $(SRC_BON)

# The formulas used to search for specific files.							#
OBJ			=	$(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJ_BON		=	$(SRC_BON:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEP			=	$(ALL_SRC:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)

# --------------------------------- Libft --------------------------------- #

LIBFT_ABS	=	$(LIBFT_DIR)/$(LIBFT)
LIBFT_DIR	=	$(SRC_DIR)/libft
LIBFT		=	libft.a

ifeq ($(LIBFT_USE), 1)
	NAME_DEPS	= $(LIB_REG) $(LIBFT)
 	NAME_LIBS	= $(LIB_REG) $(LIBFT_ABS)
	CPPFLAGS	+=	-Ilibft/include
else
	NAME_DEPS	= $(LIB_REG)
	NAME_LIBS	= $(LIB_REG)
endif

# ------------------------------- Cosmetics ------------------------------- #

C_R			=	\033[0;31m
C_G			=	\033[0;32m
C_B			=	\033[0;34m
C_C			=	\033[0;35m
C_END		=	\033[0m

# ----------------------------- Makefile Body ----------------------------- #

.PHONY: all clean fclean re bonus

all: $(NAME)
	@echo "$(C_G)[✓] $(NAME) - Program built successfully.$(C_END)"

# Creates an object directory named $@, and subdirectories.					#
$(OBJ_DIR) $(DEP_DIR) $(LIB_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(DEP_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)/shellft
	@mkdir -p $(OBJ_DIR)/parser
	@mkdir -p $(OBJ_DIR)/execution
	@mkdir -p $(OBJ_DIR)/execution/msh_cmd

# Creates the regular library.												#
$(LIB_REG): $(OBJ) | $(LIB_DIR)
	@ar rcs $@ $^

# Creates the bonus library.												#
$(LIB_BON): $(OBJ_BON) | $(LIB_DIR)
	@ar rcs $@ $^

# Compiles each %(generic).c source into its respective %(generic).o		#
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DEP_DIR) $(OBJ_DIR)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(DFLAGS) -c $< -o $@ -MF $(DEP_DIR)/$(@F:.o=.d)
	@echo "$(C_C)[OK] Compiled $<$(C_END)"

# Includes the dependencies used to verify index files changes				#
-include $(DEP)

# Creates the libft.a library.												#
ifeq ($(LIBFT_USE), 1)
$(LIBFT):
	@make -s -C $(LIBFT_DIR)
	@echo "$(C_G)[✓] Libft - Compiled.$(C_END)"
endif

# Compiles the main with the program library into the final executable.		#
$(NAME): $(NAME_DEPS) src/main.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) src/main.c -lreadline $(NAME_LIBS) -o $@

# Compiles the bonus and the program library into the bonus executable.		#
bonus: $(NAME_DEPS) $(LIB_BON) src/main_bonus.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) \
	src/main_bonus.c $(NAME_LIBS) $(LIB_BON) -o $(NAME)_bonus
	@echo "$(C_G)[✓] $(NAME)_bonus - Program built successfully.$(C_END)"

# Removes every object inside $(OBJ_DIR) and the directory itself.			#
clean:
ifeq ($(LIBFT_USE), 1)
	@make clean -s -C $(LIBFT_DIR)
endif
	@rm -rf $(OBJ_DIR)
	@echo "$(C_B)[i] $(NAME) - Cleaned object files.$(C_END)"

# Removes all the files made with this makefile.							#
fclean:
ifeq ($(LIBFT_USE), 1)
	@make fclean -s -C $(LIBFT_DIR)
endif
	@rm -rf $(OBJ_DIR)
	@rm -rf $(LIB_DIR)
	@rm -rf $(DEP_DIR)
	@rm -f $(NAME)
	@rm -f $(NAME)_bonus
	@echo "$(C_R)[x] $(NAME) - Fully cleaned project files.$(C_END)"

#- Restarts the Makefile. Erases everything to default, and executes again.	#
re: fclean all

# ----------------------------- Makefile End ------------------------------ #