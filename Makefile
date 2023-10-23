# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sebasnadu <johnavar@student.42berlin.de>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/25 11:34:58 by sebasnadu         #+#    #+#              #
#    Updated: 2023/10/23 10:58:41 by sebas_nadu       ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# COLORS
DEFAULT = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

NAME = pipex

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L $(LIBFT_DIR) -lft

INCLUDES = -I ./includes -I $(LIBFT_DIR)/includes/
CC = cc
CFLAGS = -Wall -Wextra -Werror
#CFLAGS		= -Wall -Wextra -Werror -g3 -fsanitize=address
LEAKS		= #valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes --track-origins=yes -s -q
RM = rm -f
PRINTF = printf

DIR_SRC			= ./src
DIR_SRC_BONUS 	= ./src_bonus
DIR_OBJ 		= ./obj
DIR_OBJ_BONUS	= ./obj_bonus
SRC				= $(wildcard $(DIR_SRC)/*.c)
SRC_BONUS 		= $(wildcard $(DIR_SRC_BONUS)/*.c)
OBJ				= $(SRC:$(DIR_SRC)/%.c=$(DIR_OBJ)/%.o)
OBJ_BONUS 		= $(SRC_BONUS:$(DIR_SRC_BONUS)/%.c=$(DIR_OBJ_BONUS)/%.o)

# progress bar
SRC_COUNT_TOT := $(shell expr $(shell echo -n $(SRC) | wc -w) - $(shell ls -l $(DIR_OBJ) 2>&1 | grep ".o" | wc -l))
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell expr $(shell echo -n $(SRC) | wc -w) - 1)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))

# bonus progress bar
SRCB_COUNT_TOT := $(shell expr $(shell echo -n $(SRC_BONUS) | wc -w) - $(shell ls -l $(DIR_OBJ_BONUS) 2>&1 | grep ".o" | wc -l))
ifeq ($(shell test $(SRCB_COUNT_TOT) -le 0; echo $$?),0)
	SRCB_COUNT_TOT := $(shell expr $(shell echo -n $(SRC_BONUS) | wc -w) - 1)
endif
SRCB_COUNT := 0
SRCB_PCT = $(shell expr 100 \* $(SRCB_COUNT) / $(SRCB_COUNT_TOT))

# **************************************************************************** #

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FLAGS) -o $(NAME)
	@$(PRINTF) "\r%100s\r$(GREEN)$(NAME) is up to date!$(DEFAULT)\n"

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -p $(DIR_OBJ)
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)

# bonus: $(LIBFT) $(OBJ_BONUS)
# 	@$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT_FLAGS) -o $(NAME)
# 	@$(PRINTF) "\r%100s\r$(GREEN)$(NAME) is up to date!$(DEFAULT)\n"

$(DIR_OBJ_BONUS)/%.o: $(DIR_SRC_BONUS)/%.c
	@mkdir -p $(DIR_OBJ_BONUS)
	@$(eval SRCB_COUNT = $(shell expr $(SRCB_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRCB_COUNT) $(SRCB_COUNT_TOT) $(SRCB_PCT)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	@$(PRINTF) "$(CYAN)Cleaning up object files in $(DIR_OBJ)...$(DEFAULT)\n"
	@$(RM) -rf $(DIR_OBJ)
	@$(PRINTF) "$(CYAN)Cleaning up object files in $(DIR_OBJ_BONUS)...$(DEFAULT)\n"
	@$(RM) -rf $(DIR_OBJ_BONUS)

fclean: clean
	@$(RM) $(LIBFT_DIR)/libft.a
	@$(RM) $(NAME)
	@$(PRINTF) "$(CYAN)Removed $(LIBFT)$(DEFAULT)\n"
	@$(PRINTF) "$(CYAN)Removed $(NAME)$(DEFAULT)\n"
	
re: fclean all

norminette: | $(LIBFT_DIR)
	@$(PRINTF) "$(CYAN)\nChecking norm for $(NAME)...$(DEFAULT)\n"
	@norminette -R CheckForbiddenSourceHeader $(DIR_SRC) $(DIR_SRC_BONUS) includes
	@make norminette -C libft

.PHONY: all clean fclean re norminette
